#include <stdlib.h>
#include<stdio.h>
#include "message.h"
#include<sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "logging.h"
#include <limits.h>
#include <wait.h>


#define PIPE(fd) socketpair(AF_UNIX, SOCK_STREAM, PF_UNIX, fd)


typedef struct monster {
    char symbol;
    coordinate position;
    int health;
    int damage;
    int defence;
    int range;
    bool isAlive;
    int fd[2];
    pid_t pid;
    monster_message message;
    monster_response response;
} monster;


monster monsters[MONSTER_LIMIT];

bool gameOver;

map_info map;



int manhattanDistance(int ax, int ay, int bx, int by) {
    int result;
    result = abs(ax - bx) + abs(ay - by);
    return result;
}

void monsterPrinter(monster m[], int limit) {
    int i=0;
    for(i=0; i<limit; i++) {
        printf("%c %d %d %d %d %d %d", m[i].symbol, m[i].position.x, m[i].position.y,
               m[i].health, m[i].damage, m[i].defence, m[i].range);
        printf("\n");
    }
}


void swapMonsters(int index1, int index2) {
    monster temp;
    temp = monsters[index1];
    monsters[index1] = monsters[index2];
    monsters[index2] = temp;
    map.monster_types[index1] = monsters[index1].symbol;
    map.monster_types[index2] = monsters[index2].symbol;
    map.monster_coordinates[index1] = monsters[index1].position;
    map.monster_coordinates[index2] = monsters[index2].position;
}



void sortMonsters(int monsterCount) {
    bool sorted = false;
    int i = 0, j = 0;
    while(!sorted) {
        sorted = true;
        for(j=0; j<monsterCount-1; j++) {
            for(i=0; i<monsterCount-j-1; i++) {
                if ((monsters[i].position.x > monsters[i+1].position.x) || (monsters[i].position.x == monsters[i+1].position.x && monsters[i].position.y > monsters[i+1].position.y)) {
                    swapMonsters(i, i+1);
                    sorted = false;
                }
            }
        }
    }
}


void sendDeadMonsterToEnd(int i1, int i2) {
    int i=0;
    for(i=i1; i<i2; i++) {
        swapMonsters(i, i+1);
    }
}


bool areSameCoordinates(coordinate a, coordinate b) {
    return (a.x == b.x && a.y == b.y);
}


bool checkIfEntityCanMove(coordinate target, bool isPlayer) {
    int i=0;
    if (areSameCoordinates(target, map.door)) {
        if(isPlayer) return true;
        else return false;
    }
    else if (target.x <= 0 || target.x >= map.map_width-1 || target.y <= 0 || target.y >= map.map_height-1) return false;
    else if (!isPlayer && map.door.x == target.x && map.door.y == target.y) return false;
    else {
        if (isPlayer) {
            for(i=0; i<MONSTER_LIMIT; i++) {
                if (areSameCoordinates(target, map.monster_coordinates[i])) return false;
            }
            return true;
        }
        else {
            if (areSameCoordinates(target, map.player)) return false;
            else {
                for(i=0; i<MONSTER_LIMIT; i++) {
                    if(areSameCoordinates(target, map.monster_coordinates[i])) return false;
                }
            }
        }
        return true;
    }
}



int reader(int fd, void* bf, int size) {
    // fprintf(stderr, "inside reader \n");
    int res = 1;
    fd_set readfd;

    struct timeval timeout;
    timeout.tv_sec  = 0;
    timeout.tv_usec = 1000;

    FD_ZERO(&readfd);
    FD_SET(fd, &readfd);
    int r = select(fd+1, &readfd, NULL, NULL, &timeout);

    if (r > 0) {
        res = read(fd, bf, size);
        //res = recv(fd, bf, size, MSG_DONTWAIT);
    }
    else return 5;

    return res;
}



int main(int argc, char* argv[]) {
    int i = 0, j = 0, limit = 0;
    int playerTotalDamage = 0, aliveMonsterCount = 0;


    char playerExec[100];
    char monsterExec[100];
    char monsterExecs[20][100];
    char healthBuffer[32];
    char damageBuffer[32];
    char defenceBuffer[32];
    char rangeBuffer[32];
    char doorXBuffer[32];
    char doorYBuffer[32];
    char playerMaxAttackMonsterCountBuffer[10];
    char playerMaxDistToAttackBuffer[10];
    char playerTurnNumberBuffer[10];


    for(i=0; i<MONSTER_LIMIT; i++) {
        monsters[i].pid = 22;
    }

    scanf("%d %d", &map.map_width, &map.map_height);

    scanf("%d %d", &map.door.x, &map.door.y);

    scanf("%d %d", &map.player.x, &map.player.y);
    scanf("%s %s %s %s", playerExec, playerMaxAttackMonsterCountBuffer, playerMaxDistToAttackBuffer, playerTurnNumberBuffer);
    //scanf("%*s %d %d %d", &playerMaxAttackMonsterCount, &playerMaxDistToAttack, &playerTurnNumber);
    scanf("%d", &map.alive_monster_count);



    gameOver = false;


    limit = (MONSTER_LIMIT <= map.alive_monster_count) ? MONSTER_LIMIT : map.alive_monster_count;

    for(i=0; i<limit; i++) {
        monsters[i].pid = 0;
        monsters[i].isAlive = true;
        scanf("%s %c %d %d %d %d %d %d",monsterExec, &monsters[i].symbol, &monsters[i].position.x, &monsters[i].position.y,
              &monsters[i].health, &monsters[i].damage, &monsters[i].defence, &monsters[i].range);

        map.monster_coordinates[i] = monsters[i].position;

        for(j=0; j<100; j++) {

            monsterExecs[i][j] = monsterExec[j];
        }

        map.monster_types[i] = monsters[i].symbol;

    }

    sprintf(doorXBuffer, "%d", map.door.x);
    sprintf(doorYBuffer, "%d", map.door.y);
    //sprintf(playerMaxAttackMonsterCountBuffer, "%d", playerMaxAttackMonsterCount);
    //sprintf(playerMaxDistToAttackBuffer, "%d", playerMaxDistToAttack);
    //sprintf(playerTurnNumberBuffer, "%d", playerTurnNumber);


    //monsterPrinter(monsters, limit);

    //sortMonsters(limit);

    //monsterPrinter(monsters, limit);


    game_over_status gameOverStatus;
    player_response playerResponse;
    player_message playerMessage;
    // monster_message monsterMessage;



    // establish pipes
    // fork and execute player and monsters


    pid_t player_pid;
    int fdPlayer[2];

    PIPE(fdPlayer);
    player_pid = fork();
    if(player_pid == 0) { // in child process
        fflush(stdout);
        dup2(fdPlayer[1], 1);
        dup2(fdPlayer[1], 0);
        close(fdPlayer[0]);
        close(fdPlayer[1]);
        execl(playerExec,  playerExec, doorXBuffer, doorYBuffer, playerMaxAttackMonsterCountBuffer, playerMaxDistToAttackBuffer, playerTurnNumberBuffer, (char*) 0);
    }



    pid_t worldPid = getpid();

    for (i=0; i<limit; i++) {

        sprintf(healthBuffer, "%d", monsters[i].health);
        sprintf(damageBuffer, "%d", monsters[i].damage);
        sprintf(defenceBuffer, "%d", monsters[i].defence);
        sprintf(rangeBuffer, "%d", monsters[i].range);

        PIPE(monsters[i].fd);
        // printf("file desc%d %d \n", monsters[i].fd[0], monsters[i].fd[0]);
        monsters[i].pid = fork();
        monsters[i].response.mr_type = 5;
        if (monsters[i].pid == 0) { // child so monster
            close(fdPlayer[0]);
            close(fdPlayer[1]);
            for (j=0; j<i; j++) { // todo check
                close(monsters[j].fd[0]);
                close(monsters[j].fd[1]);
            }
            fflush(stdout);
            dup2(monsters[i].fd[1], 0);
            dup2(monsters[i].fd[1], 1);
            close(monsters[i].fd[0]);
            close(monsters[i].fd[1]);

            execl(monsterExecs[i], monsterExecs[i], healthBuffer, damageBuffer, defenceBuffer, rangeBuffer, (char*) 0);
        }
    }



    // wait for ready signal from all child processes

    while(true) {
        reader(fdPlayer[0], &playerResponse, sizeof(player_response));
        if(playerResponse.pr_type == pr_ready) break;
    }



    j = 0;
    for(i=0; i<limit; i++) {
        reader(monsters[i].fd[0], &monsters[i].response, sizeof(monster_response));
        if (monsters[i].response.mr_type != mr_ready) {
            // todo terminate & exit
        }
    }

    // print initial map
    print_map(&map);









    // MAIN LOOP

    while(true) {
        // int playerFDopen = 1;
        aliveMonsterCount = map.alive_monster_count;

        if(gameOver) { // a game over cond is met
            // print_game_over(gameOverStatus);
            return 0;
            // print the last map print_map(&map);
            // print appropriate game over mssage
        }

        // send turn message player
        for(i=0; i<MONSTER_LIMIT; i++) {
            playerMessage.monster_coordinates[i] = map.monster_coordinates[i];
            // monsters[i].message.player_coordinate = map.player;
        }


        // TODO player lost by leave the game condition

        playerMessage.new_position = map.player;
        playerMessage.total_damage = playerTotalDamage;
        playerMessage.alive_monster_count = map.alive_monster_count;
        playerMessage.game_over = gameOver;


        write(fdPlayer[0], &playerMessage, sizeof(player_message));


        // receive response from player


        if (reader(fdPlayer[0], &playerResponse, sizeof(player_response)) == 5) {
            // fprintf(stderr, "inside if \n");
            gameOver = true;
            gameOverStatus = go_left;
            print_map(&map);
            print_game_over(gameOverStatus);
            for(i=0; i<map.alive_monster_count; i++) {
                kill(monsters[i].pid, SIGTERM);
                waitpid(monsters[i].pid, &i, 0);
            }
            kill(player_pid, SIGTERM);
            waitpid(player_pid, &i, 0);
            return 0;
        }




        // process response
        if (playerResponse.pr_type == pr_move) {
            // fprintf(stderr, "player wants to move move \n");
            //fprintf(stderr, "check for positions from %d %d to %d %d \n", map.player.x, map.player.y, playerResponse.pr_content.move_to.x, playerResponse.pr_content.move_to.y);
            if(checkIfEntityCanMove(playerResponse.pr_content.move_to, true)) {
                map.player = playerResponse.pr_content.move_to;
                if (map.player.x == map.door.x && map.player.y == map.door.y) {
                    gameOver = true;
                    gameOverStatus = go_reached;
                    print_map(&map);
                    print_game_over(gameOverStatus);
                    for(i=0; i<map.alive_monster_count; i++) {
                        kill(monsters[i].pid, SIGTERM);
                        waitpid(monsters[i].pid, &i, 0);
                    }
                    return 0;
                }
            }
            for(i=0; i<MONSTER_LIMIT; i++) {
                monsters[i].message.damage = 0;
            }
        }
        else if (playerResponse.pr_type == pr_attack) {
            // attack
            // fprintf(stderr, "player wants to attac \n");

            for(i=0; i<MONSTER_LIMIT; i++) {
                monsters[i].message.damage = playerResponse.pr_content.attacked[i];
            }

        }
        else if (playerResponse.pr_type == pr_dead) {
            // lose
            // fprintf(stderr, "player x< \n");
            gameOver = true;
            gameOverStatus = go_died;
            for(i=0; i<map.alive_monster_count; i++) {
                kill(monsters[i].pid, SIGTERM);
                waitpid(monsters[i].pid, &i, 0);
            }
            kill(player_pid, SIGTERM);
            waitpid(player_pid, &i, 0);
            print_map(&map);
            print_game_over(gameOverStatus);
            return 0;
        }




        // send turn message to every monster

        for(i=0; i<map.alive_monster_count; i++) {
            monsters[i].message.player_coordinate = map.player;
            monsters[i].message.game_over = gameOver;
            monsters[i].message.new_position = monsters[i].position;

            fflush(stdout);
            write(monsters[i].fd[0], &monsters[i].message, sizeof(monster_message));
            // fprintf(stderr, "monsters messages dmg: %d, player pos: %d %d, monster new pos: %d %d \n", monsters[i].message.damage, monsters[i].message.player_coordinate.x, monsters[i].message.player_coordinate.y, monsters[i].message.new_position.x, monsters[i].message.new_position.y);
        }


        for(i=0; i<map.alive_monster_count; i++) {
            // fprintf(stderr, "alive monster ct : %d \n", map.alive_monster_count);
            reader(monsters[i].fd[0], &monsters[i].response, sizeof(monster_response));
        }

// come


        // after receiving responses, and sort by coordinates (l -> r, t -> b, start from 0)
        sortMonsters(limit);
        //monsterPrinter(monsters, limit);




        // collect responses from monsters in a loop checking for any of the pipe ends for a new responses and reading them.

        playerTotalDamage = 0;


        for(i=0; i<map.alive_monster_count; i++) {
            if (monsters[i].response.mr_type == mr_move) {
                // fprintf(stderr, "monster %c is now at %d %d \n", monsters[i].symbol, monsters[i].position.x, monsters[i].position.y);
                if(checkIfEntityCanMove(monsters[i].response.mr_content.move_to, false)) {
                    monsters[i].position = monsters[i].response.mr_content.move_to;
                    map.monster_coordinates[i] = monsters[i].response.mr_content.move_to;
                    // fprintf(stderr, "Monster %c wants to go to %d %d\n", monsters[i].symbol, monsters[i].position.x, monsters[i].position.y);
                }
            }
            else if (monsters[i].response.mr_type == mr_attack) {
                //fprintf(stderr,"monster attack \n");

                // attack
                playerTotalDamage += monsters[i].damage;
            }
            else if (monsters[i].response.mr_type == mr_dead) {
                int childStatus = mr_dead;
                // kill monster
                // wait for that monster process to exit before starting a new turn.
                // fprintf(stderr, "monster dead \n");
                monsters[i].position.x = INT_MAX;
                monsters[i].position.y = INT_MAX;
                map.monster_coordinates[i].x = INT_MAX;
                map.monster_coordinates[i].y = INT_MAX;
                map.alive_monster_count--; // demo
                sendDeadMonsterToEnd(i, map.alive_monster_count);
                kill(monsters[map.alive_monster_count].pid, SIGTERM);
                waitpid(monsters[map.alive_monster_count].pid, &childStatus, 0);
                i--;
            }
            // else fprintf(stderr,"Connection problem");
        }

        // fprintf(stderr, "map alive monster count %d \n", map.alive_monster_count);
        // map.alive_monster_count = aliveMonsterCount;
        // fprintf(stderr, "map alive monster count %d \n", map.alive_monster_count);

        //sortMonsters(map.alive_monster_count);


        if (map.alive_monster_count == 0) {
            gameOver = true;
            gameOverStatus = go_survived;
            kill(player_pid, SIGTERM);
            waitpid(player_pid, &i, 0);
            print_map(&map);
            print_game_over(gameOverStatus);
            return 0;
        }

        //monsterPrinter(monsters, 20);
        print_map(&map);



    }

}

