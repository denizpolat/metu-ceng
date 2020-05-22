#include "Cell.h"
// #include "Tissue.h"

/*********************************************************************
		*									Constructor
		*
		* The first @param is the id of the cell
		* The second @param is the cell wall which defines the shape of the cell
		* The third @param is the Tissue that cell belongs to
		*/
Cell::Cell(int _id, const vector<Wall*>& _walls, Tissue* _tissue) {
    this->id = _id;
    this->tissue = _tissue;
    int s = _walls.size();
    for(int i=0; i<s; i++) {
        Wall* temp = (*_walls[i]).Clone();
        walls.push_back(temp);
    }

}
/*********************************************************************
*									Copy Constructor
*
* Deep copy
*/
Cell::Cell(const Cell& rhs) {
    this->id = rhs.id;
    int size = rhs.walls.size();
    for(int i=0; i<size; i++) {
        Wall* temp = new Wall(*(rhs.walls[i]));
        (this->walls).push_back(temp);
    }
    this->tissue = rhs.tissue;
}
/*********************************************************************
*									GetCellID
*
* @return the cell id
*/
int Cell::GetCellID() const {
    return id;
}
/*********************************************************************
*									GetCellID
*
* @return pointer for the tissue that this cell belongs to.
*/
Tissue* Cell::GetTissue() const {
    return tissue;
}
/*********************************************************************
*									GetCellWall
*
* @return the cell wall
*/
const vector<Wall*>& Cell::GetCellWall() const {
    return walls;
}
/*********************************************************************
*									RenewCellWall
*
* Replaces the current cell wall with the one given in the argument
*/
void Cell::RenewCellWall(vector<Wall*>& newWalls) {
    int currsize = walls.size();
    for(int i=0; i<currsize; i++) {
        delete walls[i];
    }
    walls = newWalls;

}
/*********************************************************************
*									StrengthenCellWall
*
* Creates a new cell wall by connecting the partial walls which can
  be added with + operator
* Replaces the old cell wall with the new one
*/
void Cell::StrengthenCellWall() {  // this does not strengthen last element with first element
    int size = walls.size();
    bool flag = false;
    if(size == 1) return;
    for(int i=0; i<size; i++) {
        for(int j=i+1; j<size; j++) {
            try {
                const Wall* x = &(*(walls[i]) + *(walls[j]));
                Wall* wall = x->Clone();
                delete walls[i];
                delete walls[j];
                walls.erase(walls.begin() + j);
                walls.erase(walls.begin() + i);
                walls.push_back(wall);
                delete x;
                flag = true;
                size--;
                break;
            }
            catch (ApplePearException e) {
                continue;
            }
        }
        if(flag) break;
    }
    if(flag) StrengthenCellWall();
}
/*********************************************************************
*									Destructor
*
*/
Cell::~Cell() {
    int s = walls.size();
    for(int i=0; i<s; i++) {
        delete walls[i];
    }
    tissue = NULL;
}

/*********************************************************************
*									Stream Operator
*
* Prints cell wall
* The format is: (p1.x,p1.y) - (p2.x,p2.y) - ... - (pn.x,pn.y)
  where p1, p2, ..., pn are the the particles on the cell wall.
* Do NOT add new line character "\n" to the end of the stream.
*/
ostream& operator<<(ostream& os, const Cell& cell) {
    int size = cell.walls.size(), i=0;
    os << cell.walls[i]->GetInitialParticle() ;
    for(i=1; i<size; i++) {
        os << " - " << cell.walls[i]->GetInitialParticle();
    }
    return os;
}
/********************************************************************/