#include "Circular.h"


/*********************************************************************
		*									Constructor
		*
		* The first @param is the id of the cell
		* The second @param is the radius of the boundary
		*/
Circular::Circular(int id, float radius) : MicroOrganism(id, vector<Wall*>()) {
    this->id = id;
    Wall* wall = new CurvyWall(2*PI*radius, radius);
    this->walls.push_back(wall);
    this->radius = radius;
    this->center = NULL;
    this->connectedCell = NULL;

}
/*********************************************************************
*									Destructor
*
*/
Circular::~Circular() {
    walls.erase(walls.begin());
    delete walls[0];
    delete center;
    //delete connectedCell;
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
void Circular::ConnectToCell(Cell* const cell) {
    this->connectedCell = cell;
    cell->StrengthenCellWall();
    CurvyWall* temp = dynamic_cast<CurvyWall *>(cell->GetCellWall()[0]); //this line is so important !!!!! if u wanna find again, search "importantelicious"
    this->center = new Particle(temp->GetCenter());
    Particle init = Particle(temp->GetInitialParticle());
    int s = walls.size();
    for(int i=0; i<s; i++) {
        delete walls[0];
        walls.erase(walls.begin());
    }
    CurvyWall* c = new CurvyWall(init, init, temp->GetCenter());
    walls.push_back(c);

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
bool Circular::DoesFitInto(const Cell& cell) const {
    CurvyWall* temp = dynamic_cast<CurvyWall *>(cell.GetCellWall()[0]);
    if(!temp || connectedCell) return false;
    float tempradius = temp->GetRadius();
    return (abs(tempradius - this->radius) <= EPSILON);
}
/*********************************************************************
*										React
*
* This type of microorganism makes the size of the cell get two times
  larger without changing the center of the cell.
* As an effect of the enlarging, you should renew the cell walls.
* The microorganism size does not change.
*/
void Circular::React() {
    int new_r = radius*2;
    Particle initialPoint = Particle(center->X(), center->Y() + new_r);
    Particle c = Particle(*this->center);
    Wall* newWall = new CurvyWall(initialPoint, initialPoint, c);
    vector<Wall*> newCircle;
    newCircle.push_back(newWall);
    this->connectedCell->RenewCellWall(newCircle);
}
/*********************************************************************
*									DoesContain
*
* Special to this type of microorganism
* Checks whether the given Particle in the @param is inside the
  this microorganism, or not.
* @return true if the particle is inside, false otherwise.
*/
bool Circular::DoesContain(const Particle& point) const {
    if(!center) return false;
    Wall centerToPoint = Wall(*center, point);
    Wall x_axis = Wall(Particle(0,0), Particle(0,2));
    float cosine = cos(x_axis.FindAngleBetween(centerToPoint));
    float distance = cosine * centerToPoint.GetLength();
    return ((distance - this->radius) <= EPSILON);
}
/*********************************************************************/