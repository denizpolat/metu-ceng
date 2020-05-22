#include "NucleoBase.h"


/*********************************************************************
*									Constructor
*
* The first @param is the whole RNA sequence
* The second parameter is a NucleoBase pointer addressing the previous
  NucleoBase object. For the initial nucleobase this is NULL.
*/
NucleoBase::NucleoBase(string rna, NucleoBase* _prev) {
    int s = rna.size() - 1;
    if(!_prev) {
        order = 0;
        prev = NULL;
    }
    else {
        order = _prev->order + 1;
        prev = _prev;
        this->prev->next = this;
    }
    _rna = rna;
    base = rna[order];
    this->next = NULL;
    if(order < s) this->next = new NucleoBase(rna, this);
    else;
}
/*********************************************************************
*								Copy Constructor
*
* Deep copy
*/
NucleoBase::NucleoBase(const NucleoBase& rhs) {
    this->_rna = rhs._rna;
    this->order = 0;
    this->prev = NULL;
    this->base = this->_rna[0];
    this->next = new NucleoBase(_rna, this);

}
/*********************************************************************
*									Destructor
*
*/
NucleoBase::~NucleoBase() {
    NucleoBase* tmp = this;
    if(!tmp->next) return;
    else {
        tmp->next->prev = NULL;
        delete tmp->next;
    }
}
/*********************************************************************
*									GetLast
*
* @return pointer to the last element of NucleoBase chain.
*/
NucleoBase* NucleoBase::GetLast() {
    NucleoBase* temp = this;
    while(temp->next) temp = temp->next;
    return temp;
}
/*********************************************************************
*									Stream Operator
* Prints the data in NucleoBase chain.
* Prints the char data of each NucleoBase object starting from the
  given NucleoBase in the @param upto the last element of the chain.
* Do NOT add new line character "\n" to the end of the stream.
*/
ostream& operator<<(ostream& os, const NucleoBase& nucleoBase) {
    NucleoBase* temp = const_cast<NucleoBase *>(&nucleoBase);
    while(temp) {
        os << temp->base;
        temp = temp->next;
    }
    return os;
}
/********************************************************************/