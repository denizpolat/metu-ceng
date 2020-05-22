#include "Tissue.h"

/*********************************************************************
		*									Constructor
		*
		*/
Tissue::Tissue() {
    xmin = -999999;
    ymin = xmin;
    ymax = 999999;
    xmax = 999999;
}
/*********************************************************************
*									Constructor
*
* The first @param is the minimum x border
* The second @param is the maximum x border
* The third @param is the minimum y border
* The fourth @param is the maximum y border
* The tissue is the region residing among those coordinates.
*/
Tissue::Tissue(float min_x, float max_x, float min_y, float max_y) {
    xmin = min_x;
    xmax = max_x;
    ymin = min_y;
    ymax = max_y;
}
/*********************************************************************
*								Copy Constructor
*
* Shallow copy
*/
Tissue::Tissue(const Tissue& rhs) {
    xmin = rhs.xmin;
    xmax = rhs.xmax;
    ymin = rhs.ymin;
    ymax = rhs.ymax;
    connectedCells = rhs.connectedCells;
}
/*********************************************************************
*									Destructor
*
*/
Tissue::~Tissue() {
    int s = connectedCells.size();
    for(int i=0; i<s; i++) {
        delete connectedCells[0];
        connectedCells.erase(connectedCells.begin());
    }
}
/*********************************************************************
*									GetCoordinates
*
* @return the coordinates of minimum x border, maximum x border,
  minimum y border and maximum y border in an array with the stated
    order.
*/
float* Tissue::GetCoordinates() {
    float* result;
    result[0] = xmin;
    result[1] = xmax;
    result[2] = ymin;
    result[3] = ymax;
    return result;
}
/*********************************************************************
*									AddCell
*
* Adds the actual reference of the cell given in the @param to storage
  of cells belonging to this Tissue object.
*/
void Tissue::AddCell(Cell& cell) {
    connectedCells.push_back(&cell);
}
/*********************************************************************
*									RemoveCell
*
* Deletes the cell given in the @param and removes it from the storage
  of cells belonging to this Tissue object.
* Do the necessary operation that your own implementation requires.
    (Whatever it is: Like decreasing the number of cells, or erasing from
    storage that you used to hold the cells, or anything else...)
*/
void Tissue::RemoveCell(Cell& cell) {
    int s = connectedCells.size();
    for(int i=0; i<s; i++) {
        if((connectedCells)[i]->GetCellID() == cell.GetCellID()) {
            // this function does NOT erase the tissue from cell
            connectedCells.erase(connectedCells.begin() + i);
        }
    }
}
/*********************************************************************
*									GetCell
*
* @return the cell whose id is given in the @param
* If the cell whose id is given does not exit, in other words it is a
  dead cell, then the method should throw DeadCellException which was
    defined in Exception.h.
*/
const Cell& Tissue::GetCell(int id) const {
    DeadCellException d;
    int s = connectedCells.size();
    for(int i=0; i<s; i++) {
        if(connectedCells[i]->GetCellID() == id) return *connectedCells[i];
    }
    throw d;
}
/*********************************************************************
*									Interact
*
* This method makes the cells interacting with the microorganisms in
  the @param. For each microorganism a compatible cell is found.
  Input will be given satisfying that each microorganism fits into a cell.
*/
void Tissue::Interact(const vector<MicroOrganism*>& mic) {
    int micsize = mic.size(), cellsize = connectedCells.size();
    for(int i=0; i<micsize; i++) {
        for(int j=0; j<cellsize; j++) {
            if(mic[i]->DoesFitInto(*connectedCells[j])) {
                mic[i]->ConnectToCell(connectedCells[j]);
                break;
            }
        }
    }
}

/*********************************************************************/

void Tissue::CreatePartitions(const vector<MicroOrganism*>&) {}

void Tissue::TreatByPlasma(Tissue) {}
