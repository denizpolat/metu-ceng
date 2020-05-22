#include "MicroOrganism.h"

/*********************************************************************
		*									Constructor
		*
		* The first @param is the microorganism id
		* The second @param is the edges of Wall type
		  The given edges will have been constructed by their lengths and radius
		  (without a definite location)
		*/
//MicroOrganism::MicroOrganism() = default;


MicroOrganism::MicroOrganism(int _id, const vector<Wall*>& edges) {
    this->id = _id;
    int s = edges.size();
    for(int i=0; i<s; i++) {
        this->walls.push_back(new Wall(edges[i]->GetLength()));
    }
    connectedCell = NULL;
}


/*********************************************************************
*									Stream Operator
*
* Prints the location of the microorganism
* If it is located in a cell, print in the following format:
  "The microorganism <id1> was successfully placed into the cell <id2>."
  where id1 is the microorganism id and id2 is the cell id.
  If the microorganism is not located into any cell, then print:
  "The microorganism <id> could not be placed into any cell!"
* Do NOT add new line character "\n" to the end of the stream.
*/
ostream& operator<<(ostream& os, const MicroOrganism& micro) {
    if(micro.connectedCell) {
        os << "The microorganism " << micro.id << " was successfully placed into the cell " << micro.connectedCell->GetCellID() << ".";
    }
    else {
        os << "The microorganism " << micro.id << " could not be placed into any cell!";
    }
    return os;
}

MicroOrganism::~MicroOrganism() {
    int size = walls.size();
    for(int i=0; i<size; i++) {
        delete walls[0];
        walls.erase(walls.begin());
    }
}
