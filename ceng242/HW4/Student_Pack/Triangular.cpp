#include "Triangular.h"


/*********************************************************************
*									Constructor
*
* The first @param is the id of the cell
* The second @param is the vector of edges
* The third parameter is a NucleoBase object that defining its RNA.
*/
Triangular::Triangular(int _id, const vector<Wall*>& edges, const NucleoBase& nucleoBase) : MicroOrganism(_id, edges){
    this->rnaChain = new NucleoBase(nucleoBase);
}
/*********************************************************************
*									Destructor
*
*/
Triangular::~Triangular() {
    delete this->rnaChain;
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
void Triangular::ConnectToCell(Cell* const cell) {
    cell->StrengthenCellWall();
    this->connectedCell = cell;
    int x = this->walls.size();
    for(int i=0; i<x; i++) {
        delete walls[0];
        walls.erase(walls.begin());
    }
    for(int i=0; i<3; i++) {
        Wall* newWall = new Wall(*cell->GetCellWall()[i]);
        walls.push_back(newWall);
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
bool Triangular::DoesFitInto(const Cell& cell) const {
    if(this->connectedCell) return false;
    Cell c = Cell(cell);
    c.StrengthenCellWall();
    if(c.GetCellWall().size() != 3) return false;
    bool isCheckedTriangle[3];
    bool isCheckedCell[3];
    for(int i=0; i<3; i++) {
        isCheckedTriangle[i] = false;
        isCheckedCell[i] = false;
    }
    for(int i=0; i<3; i++) { // i for cell
        for(int j=0; j<3; j++) { // j for triangle
            if((abs(this->walls[j]->GetLength() - c.GetCellWall()[i]->GetLength()) <= EPSILON) && !isCheckedCell[i] && !isCheckedTriangle[j]) {
                isCheckedCell[i] = true;
                isCheckedTriangle[j] = true;
                break;
            }
        }
    }
    for(int i=0; i<3; i++) {
        if(!isCheckedTriangle[i] || !isCheckedCell[i]) return false;
    }
    return true;
}
/*********************************************************************
*										React
*
* This type of microorganism kills the cell that it connects to.
* Note that while you are deleting the cell, you should also make the
* Tissue noticed that the corresponding cell was killed.
*/
void Triangular::React() {
    if(this->connectedCell) this->connectedCell->GetTissue()->RemoveCell(*this->connectedCell);
    delete this->connectedCell;

}
/*********************************************************************
*									GetRNA
*
* Special to this type of microorganism
* @return the start node of the NucleoBase sequence which holds the RNA
  of the microorganism
*/
const NucleoBase& Triangular::GetRNA() const {
    NucleoBase* rna = this->rnaChain;
    while(rna->prev) rna = rna->prev;
    return *rna;
}
/*********************************************************************
*									Mutate
*
* Special to this type of microorganism
* Mutate the RNA of this Triangular microorganism by crossing over with
  RNA of the Triangular microorganism given in the @param.
* Crossing over is done as follows:
  Both RNA is compared in both ways: from the beginning to end and from
  end to beginning. When they start to differ, the corresponding G-S
  and A-U nucleobases in the differing part of both RNA are interchanged.
  For example:

  - Let RNA-1 be AGSGGAUU AAUGASGUS UAG
  - Let RNA-2 be AGSGGAUU UASGAGSAA UAG

  -result: AGSGGAUU ASGU UAG
   GSGGAUU ASGU .A.UAG

  Since ASGAG part at the beginning and UAGSSSAAG part at the end are
  the same in both RNA, those parts are held. For the parts remaining
  in the middle, the corresponding G-S nucleobases and the corresponding
  A-U nucleobases are interchanged. Other type of correspondences are
    removed from the chain (Deleted). At the end:
  - RNA-1 becomes ASGAGSAUAGSSSAAG
    (make readable: ASGAG SA UAGSSSAAG)
  - RNA-2 becomes ASGAGGUUAGSSSAAG
    (make readable: ASGAG GU UAGSSSAAG)
* You can assume that at least 1 character from the beginning and the end
    will be the same for both RNA sequence (Just for you not to struggle
    with the so detailed implementation). Also, you can assume that at least
    1 character of RNAs will differ.
*/
void Triangular::Mutate(Triangular& rhs) {
    NucleoBase* beginThis = this->rnaChain;
    NucleoBase* endThis = this->rnaChain->GetLast();
    NucleoBase* beginRhs = rhs.rnaChain;
    NucleoBase* endRhs = rhs.rnaChain->GetLast();
    while(beginThis->prev) beginThis = beginThis->prev;
    while(beginRhs->prev) beginRhs = beginRhs->prev;

    while(beginThis->base == beginRhs->base) {              //beginX is @theNucleoBase where the difference begins
        beginRhs = beginRhs->next;
        beginThis = beginThis->next;
    }
    while(endThis->prev->base == endRhs->prev->base) {       // endX is @theNucleoBase where the similarity in opposite direction ends.
        endRhs = endRhs->prev;
        endThis = endThis->prev;
    }
    while(beginRhs != endRhs) {
        if((beginRhs->base == 'G' && beginThis->base == 'S') || (beginRhs->base == 'S' && beginThis->base == 'G') ||(beginRhs->base == 'A' && beginThis->base == 'U') || (beginRhs->base == 'U' && beginThis->base == 'A')) {
            char temp = beginThis->base;
            beginThis->base = beginRhs->base;
            beginRhs->base = temp;
            beginRhs = beginRhs->next;
            beginThis = beginThis->next;
        }
        else {
            NucleoBase* t = beginThis;
            beginThis->prev->next = beginThis->next;
            beginThis->next->prev = beginThis->prev;
            beginThis = beginThis->next;
            t->prev = NULL;
            t->next = NULL;
            delete t;
            t = beginRhs;
            beginRhs->prev->next = beginRhs->next;
            beginRhs->next->prev = beginRhs->prev;
            beginRhs = beginRhs->next;
            t->prev = NULL;
            t->next = NULL;
            delete t;
        }
    }

}
/*********************************************************************/