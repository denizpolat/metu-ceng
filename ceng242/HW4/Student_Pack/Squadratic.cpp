#include "Squadratic.h"

/*********************************************************************
		*									Constructor
		*
		* The first @param is the id of the cell
		* The second @param is the edge length in the boundary
		*/
Squadratic::Squadratic(int _id, float edgeLen) : MicroOrganism(_id, vector<Wall*>()) {
    this->edgeLength = edgeLen;
    for(int i=0; i<4; i++) {
        Wall* oneEdge = new Wall(edgeLen);
        this->walls.push_back(oneEdge);
    }
    this->isDead = false;
    this->parent = NULL;
}
/*********************************************************************
*									Copy Constructor
*
* Deep copy
*/
Squadratic::Squadratic(const Squadratic& rhs) : MicroOrganism(rhs.id, vector<Wall*>()) {
    int s = rhs.walls.size();
    for(int i=0; i<s; i++) {
        Wall* wall = new Wall(*(rhs.walls[i]));
        this->walls.push_back(wall);
    }
    this->edgeLength = rhs.edgeLength;
    this->connectedCell = rhs.connectedCell;
    this->isDead = rhs.isDead;
    this->parent = rhs.parent;
}
/*********************************************************************
*									Destructor
*
*/
Squadratic::~Squadratic() {
    int s = walls.size();
    for(int i=0; i<s; i++) {
        delete walls[0];
        walls.erase(walls.begin());
    }
    while(this->children.size()) {
        delete children[0];
        children.erase(children.begin());
    }
}


/*********************************************************************
*									ConnectToCell
*
* Connects the microorganism to the cell given in the @param
* It is assummed that cell-microorganism comptability was checked
  before calling this method, and the cell is compatible with
  the microorganism.
* Saves the cell pointer.
* After connection, microorganism has a definite position which is
  the same location the cell is on. It is your call to save other
  connection effects like updating the walls/edges of the microorganism
  with the definitely-located walls or defining vertices of it.
*/

void Squadratic::ConnectToCell(Cell* const cell) {
        cell->StrengthenCellWall();
        this->connectedCell = cell; // COMEBACK
        int s = cell->GetCellWall().size(), x = this->walls.size();
        for(int i=0; i<x; i++) {
            delete walls[0];
            walls.erase(walls.begin());
        }
        for(int i=0; i<s; i++) {
            Wall* wall = new Wall(*(cell->GetCellWall()[i]));
            walls.push_back(wall);
        }
}
/*********************************************************************
    *									DoesFitInto
    *
    * Checks whether the microorganism fits into the cell given in
      the @param.
    * @return true if it fits, false otherwise.
    * A microorganism fits into a cell only if their boundary are the same
      (the same type like triangle, circle or square and the same size).
    * Note that boundary of a cell is always given in counterclockwise, yet
      edges of a microorganism do not have an order since it is not located
      until connecting to a compatible cell. Therefore, a triangular
      microorganism whose edges of length 3, 5, 4 units respectively may
      connect to a triangle cell whose cell wall is defined with the walls
      of length 3, 4, 5. You can consider it as turning the other face of
      the microorganism to the front.
    * Also, do not forget to check the angels between the edges of a cell
      while trying to place Squadratics. Checking only the edge lengths
      is not sufficient since edges of equilateral quadrangles are also
      equal, yet the inner angles are not 90 degrees.
    * Hint: Strengthen cell walls before checking its edges and angles.
    */
bool Squadratic::DoesFitInto(const Cell& cell) const {
    if(this->connectedCell) return false;
    Cell tempCell = Cell(cell);
    tempCell.StrengthenCellWall();
    if(tempCell.GetCellWall().size() != 4) return false;
    for(int i=0; i<4; i++) {
        float oneEdge = tempCell.GetCellWall()[i]->GetLength();
        if(abs(oneEdge - edgeLength) > EPSILON) return false;
        float angleBetween = tempCell.GetCellWall()[i]->FindAngleBetween(*(tempCell.GetCellWall()[(i+1)%4]));
        if (abs(angleBetween - (PI / 2)) <= EPSILON) continue;
        else return false;
    }
    return true;
}
/*********************************************************************
*									React
*
* This type of microorganism proceeds into meiosis division as follows:
  The current squadratic microorganism becomes a parent of 4 children
  such that each child is a new squadratic microorganism whose edge
  length is half of the parent organism's edge length.
* Id of the child microorganisms does not matter. Give anything you
  want different than the previous microorganism ids.
* Each child squadratic positions into a different corner of the cell
  such that one of their top left, bottom left, top right and bottom
  right corners match with one of the corresponding corner of the cell.
* React() method can also be called for child microorganisms.
*/

void Squadratic::React() {
    connectedCell->StrengthenCellWall();
    float centerX = (this->walls[0]->GetInitialParticle().X() + this->walls[2]->GetInitialParticle().X()) / 2;
    float centerY = (this->walls[0]->GetInitialParticle().Y() + this->walls[2]->GetInitialParticle().Y()) / 2;
    Particle center = Particle(centerX, centerY);
    for(int i=0; i<4; i++) {float id = this->id * 10 + i + 1;
        Squadratic* m = new Squadratic(id, this->edgeLength / 2);
        for(int x=0; x<4; x++) {
            delete m->walls[0];
            m->walls.erase(m->walls.begin());
        }
        int k = (i+1)%4;
        Particle middle1 = Particle(((walls[i]->GetInitialParticle().X() + walls[i]->GetFinalParticle().X()) / 2), ((walls[i]->GetInitialParticle().Y() + walls[i]->GetFinalParticle().Y()) / 2));
        Particle middle2 = Particle(((walls[k]->GetInitialParticle().X() + walls[k]->GetFinalParticle().X()) / 2), ((walls[k]->GetInitialParticle().Y() + walls[k]->GetFinalParticle().Y()) / 2));

        m->walls.push_back(new Wall(middle1, this->walls[i]->GetFinalParticle()));
        m->walls.push_back(new Wall(this->walls[i]->GetFinalParticle(), middle2));
        m->walls.push_back(new Wall(middle2, center));
        m->walls.push_back(new Wall(center, middle1));
        m->connectedCell = this->connectedCell;
        m->parent = this;
        Squadratic* tmp = m;
        this->children.push_back(m);

    }
    this->isDead = true;
}
/*********************************************************************
*									GetChild
*
* Special to this type of microorganism
* @return the child microorganism which is located between the minimum
  and maximum x and y coordinates given in the @param.
* The first @param is the minimum x border.
* The second @param is the maximum x border.
* The third @param is the minimum y border.
* The fourth @param is the maximum y border.
* This method either directly returns the child whose border coordinates
  exactly equal to the parameters, or throws NotBornChildException in
    case that the asked child was created yet (the microorganism may
  have not been divided upto that level.)
*/
Squadratic& Squadratic::GetChild(float xmin, float xmax, float ymin, float ymax) const {
    NotBornChildException bc;
    if(this->isDead) {
        int childrenSize = this->children.size();
        for(int i=0; i<childrenSize; i++) {
            Squadratic* tmp = children[i];
            float xmn = (tmp->walls[0]->GetFinalParticle().X() < tmp->walls[2]->GetFinalParticle().X()) ? tmp->walls[0]->GetFinalParticle().X() : tmp->walls[2]->GetFinalParticle().X();
            float xmx = (xmn == tmp->walls[2]->GetFinalParticle().X()) ? tmp->walls[0]->GetFinalParticle().X() : tmp->walls[2]->GetFinalParticle().X();
            float ymn = (tmp->walls[0]->GetFinalParticle().Y() < tmp->walls[2]->GetFinalParticle().Y()) ? tmp->walls[0]->GetFinalParticle().Y() : tmp->walls[2]->GetFinalParticle().Y();
            float ymx = (ymn == tmp->walls[2]->GetFinalParticle().Y()) ? tmp->walls[0]->GetFinalParticle().Y() : tmp->walls[2]->GetFinalParticle().Y();
            if( (xmn >= xmin) && (ymn >= ymin) && (xmx <= xmax) && (ymx <= ymax) ) return *tmp;
                tmp->GetChild(xmin,xmax,ymin,ymax);
        }
    }
    throw NotBornChildException(bc);
}

/*********************************************************************/
