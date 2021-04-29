#include <stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include "message.h"



int health = 0, damage = 0, defence = 0, range = 0;


int manhattanDistance(coordinate monster, coordinate player) {
    int ax = monster.x, ay = monster.y, bx = player.x, by = player.y;
    int result;
    result = abs(ax - bx) + abs(ay - by);
    return result;
}


int max(int a, int b) {
    return a > b ? a : b;
}


coordinate chooseFromPossibleDistances(coordinate monster, coordinate difference, coordinate player) {
    coordinate p1 = monster, p2 = monster, p3 = monster;
    char direction = ' '; // 0 -> top right , 1 -> top left, 2 -> bottom right, 3 -> bottom left
    if(difference.y < 0) {
        if(difference.x < 0) { // top left
            p1.y = p1.y - 1;
            p2.x = p2.x - 1;
            p3.x = p3.x - 1;
            p3.y = p3.y - 1;
            direction = 1;
        }
        else {
            p1.y = p1.y - 1;
            p2.x = p2.x + 1;
            p3.x = p3.x + 1;
            p3.y = p3.y - 1;
            direction = 0;
        }
    }
    else {
        if(difference.x < 0) {
            p1.y = p1.y + 1;
            p2.x = p2.x - 1;
            p3.x = p3.x - 1;
            p3.y = p3.y + 1;
            direction = 3;
        }
        else {
            p1.y = p1.y + 1;
            p2.x = p2.x + 1;
            p3.x = p3.x + 1;
            p3.y = p3.y + 1;
            direction = 2;
        }
    }

    //fprintf(stderr,"p1 %d %d \n", p1.x, p1.y);
    //fprintf(stderr,"p2 %d %d \n", p2.x, p2.y);
    //fprintf(stderr,"p3 %d %d \n", p3.x, p3.y);


    int posDist1 = manhattanDistance(p1, player);
    int posDist2 = manhattanDistance(p2, player);
    int posDist3 = manhattanDistance(p3, player);
    // fprintf(stderr,"possible distances %d %d %d \n", posDist1, posDist2, posDist3);



    // pos1 up-down , pos2 l-r, pos3 cross

    if (posDist1 < posDist2) {
        if (posDist1 < posDist3) return p1;
        else if(posDist3 < posDist1) return p3;
        else {
            return (direction == 2) ? p3 : p1;
            // choose between p1 and p3
        }
    }
    else if (posDist2 < posDist1) {
        if (posDist2 < posDist3) return p2;
        else if (posDist3 < posDist2) return p3;
        else {
            return (direction == 1 || direction == 2) ? p2 : p3;
            // choose between p2 and p3
        }
    }
    else {
        if (posDist3 < posDist2) return p3;
        else if (posDist2 < posDist3) {
            return (direction == 2) ? p2 : p1;
            // choose between p1 and p2
        }
        else {
            // choose between p1 p2 p3
            return (direction == 2) ? p2 : p1;
        }
    }
}


coordinate moveMonsterToPlayer(coordinate m, coordinate p) {
    coordinate difference = {p.x - m.x, p.y - m.y};
    coordinate newPos = m;


    if (difference.x != 0 && difference.y != 0) {
        // possible multiple moves
        // fprintf(stderr,"monster multiple possible move \n");


        newPos = chooseFromPossibleDistances(m, difference, p);

        // fprintf(stderr,"monster newPos : %d %d \n", newPos.x, newPos.y);

    }
    else if(difference.x != 0) {
        if (difference.x < 0) {
            newPos.x = newPos.x - 1; // to left
        }
        else {
            newPos.x = newPos.x + 1; // to right
        }
    }
    else if(difference.y != 0) {
        if (difference.y < 0) {
            newPos.y = newPos.y - 1; // to up
        }
        else {
            newPos.y = newPos.y + 1; // to down
        }
    }
    return newPos;
}






int main(int argc, char *argv[]) {
    if(argc != 5) {
        fprintf(stderr,"Error: Wrong number of parameters :(\n");
    }


    health = atoi(argv[1]);
    damage = atoi(argv[2]);
    defence = atoi(argv[3]);
    range = atoi(argv[4]);

    // printf("Hello world! \n %d %d %d %d \n", health, damage, defence, range);

    monster_response monsterResponse;
    monsterResponse.mr_type = mr_ready;

    //printf("monster will send %d \n", monsterResponse.mr_type);





    write(1, &monsterResponse, sizeof(monster_response));




    monster_message monsterMsg; // todo take it from pipe or anything else dont know


    while (true) {

        read(0, &monsterMsg, sizeof(monster_message)); // todo check



        if (monsterMsg.game_over) {
            // fprintf(stderr, "gameover signal came to monster");
            // terminate monster process
            // monsterResponse.mr_type = mr_dead;
            write(1, &monsterResponse, sizeof(monster_response));
            exit(mr_dead);
        }
        if (monsterMsg.damage > 0) { // player attacked
            health -= max(0, monsterMsg.damage - defence);
            // fprintf(stderr,"monster health inside monster: %d \n", health);

            if (health <= 0) {
                monsterResponse.mr_type = mr_dead;
                write(1, &monsterResponse, sizeof(monster_response));
                exit(mr_dead);
                // send dead signal mr_dead
                // terminate
            }
        }

        coordinate monsterPos = monsterMsg.new_position;
        coordinate playerPos = monsterMsg.player_coordinate;

        int manDist = manhattanDistance(monsterPos, playerPos);

        if(manDist <= range) {
            // fprintf(stderr, "monster can attac");
            monsterResponse.mr_type = mr_attack;
            monsterResponse.mr_content.attack = damage;
            // monster can attack
        }

        else {
            // monster moves
            monsterPos = moveMonsterToPlayer(monsterPos, playerPos);
            monsterResponse.mr_type = mr_move;
            monsterResponse.mr_content.move_to = monsterPos;
        }


        fflush(stdout);
        write(1, &monsterResponse, sizeof(monster_response));

    }



}