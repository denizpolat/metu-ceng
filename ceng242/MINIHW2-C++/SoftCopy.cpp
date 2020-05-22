#include "SoftCopy.h"
/* YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. DO NOT MODIFY THE HEADER FILE AS IT WILL BE
REPLACED BY THE ORIGINAL HEADER DURING TESTING. THE REQUIRED
HEADER FILE IS ALREADY INCLUDED FOR YOU ABOVE THIS COMMENT BLOCK.
THE CONSTRUCTOR and DESTRUCTOR ARE ALREADY IMPLEMENTED.
START YOUR IMPLEMENTATIONS BELOW THOSE. */


// DONE
SoftCopy::SoftCopy(int ISBN, double price, int startPage, int endPage) : Book(ISBN, price) {
    this->startPage = startPage;
    this->endPage = endPage;
    this->numberOfPages = (endPage - startPage) + 1;
    this->firstHalf = NULL;
    this->secondHalf = NULL;
    this->pageContent = "";
}

// DONE
SoftCopy::~SoftCopy() {
    if (firstHalf != NULL)
        delete firstHalf;
    if (secondHalf != NULL)
        delete secondHalf;
}


SoftCopy::SoftCopy(const SoftCopy& rhs) {
    this->ISBN = rhs.ISBN;
    this->price = rhs.price;
    this->startPage = rhs.startPage;
    this->endPage = rhs.endPage;
    this->numberOfPages = rhs.numberOfPages;
    this->pageContent = rhs.pageContent;
    if(rhs.firstHalf) this->firstHalf = new SoftCopy(*rhs.firstHalf);
    else firstHalf = NULL;
    if(rhs.secondHalf) this->secondHalf = new SoftCopy(*rhs.secondHalf);
    else secondHalf = NULL;
    this->next = rhs.next;
}
/*********************************************************************
*	Assignment operator
*
*  All is deep copy except the pointer variable 'next'
*/
SoftCopy& SoftCopy::operator=(const SoftCopy& rhs) {
    if(&rhs == this) return *this;
    if(firstHalf) delete firstHalf;
    firstHalf = NULL;
    if(secondHalf) delete secondHalf;
    secondHalf = NULL;
    this->ISBN = rhs.ISBN;
    this->price = rhs.price;
    this->startPage = rhs.startPage;
    this->endPage = rhs.endPage;
    this->numberOfPages = rhs.numberOfPages;
    this->pageContent = rhs.pageContent;
    if(rhs.firstHalf) this->firstHalf = new SoftCopy(*rhs.firstHalf);
    else firstHalf = NULL;
    if(rhs.secondHalf) this->secondHalf = new SoftCopy(*rhs.secondHalf);
    else secondHalf = NULL;
    this->next = rhs.next;
}
/*********************************************************************
*	Operator Overload +
*
* Adds the pages given in the @param to this SoftCopy object.
* See the pdf for the example and explanation.
* You should create a new SoftCopy object in the heap, such that it
  includes the result of addition. In the final, you should return
  that newly created heap object.
  The deletion of the returned heap object will be later done by the
  admin of the homework.
* Apply deep copy while adding the pages if you are implementing it via
  copy operations.
* Note: Do not touch pointer variable 'next' here.
*/


SoftCopy& SoftCopy::operator+(const SoftCopy& rhs) const {
    SoftCopy* rhsTraverse = const_cast<SoftCopy *>(&rhs);
    vector<SoftCopy*> rhsVector;
    vector<SoftCopy*> toAdd;
    while(rhsTraverse || !rhsVector.empty()) {
        while (rhsTraverse) {
            rhsVector.push_back(rhsTraverse);
            rhsTraverse = rhsTraverse->firstHalf;
        }
        rhsTraverse = rhsVector[rhsVector.size()-1];
        rhsVector.pop_back();
        if(rhsTraverse->numberOfPages == 1) toAdd.push_back(rhsTraverse);
        rhsTraverse = rhsTraverse->secondHalf;
    }
    int s = toAdd.size();
    SoftCopy* result = new SoftCopy(*this);
    for(int i=0; i<s; i++) {
        result->UploadPage(toAdd[i]->pageContent, toAdd[i]->startPage);
    }
    return *result;
}
/*********************************************************************
*	Clone
*
* @return pointer to a copy of this Book object
*/
Book* SoftCopy::Clone() const {
    SoftCopy* s = new SoftCopy(*this);
    return s;
}
/*********************************************************************
*	Discount
*
* Discounts the price of the book.
* It applies 25% of discount. For example:
  - If the price is 100 TL, the new price will be 75 TL.
  - If the price is 60 TL, the new price will be 60 * (75/100) = 45 TL.
*/
void SoftCopy::Discount() {
    this->price = price * (0.75);
}
/*********************************************************************
*	UploadPage
*
* Constructs the page whose id is given in the second @param, then
  Uploads the page content given in the first @param.
* See the pdf for the example and explanation.
*/
void SoftCopy::UploadPage(string content, int page) {
    int right = numberOfPages / 2;
    int left = numberOfPages - right;
    if(numberOfPages == 1) {
        if(this->startPage == page) return;
    }
    else if(right == 1) {
        if (this->endPage == page) {
            this->secondHalf = new SoftCopy(this->ISBN, this->price, page, page); // isbn, price,startpage, endpage
            this->secondHalf->pageContent = content;
        }
        else {
            if(left == 1) {
                if(this->startPage == page) {
                    this->firstHalf = new SoftCopy(this->ISBN, this->price, page, page); // isbn, price,startpage, endpage
                    this->firstHalf->pageContent = content;
                }
            }
            else {
                int middle = startPage + left - 1;
                if(!firstHalf) this->firstHalf = new SoftCopy(ISBN, price, startPage, middle);
                firstHalf->UploadPage(content, page);
            }
        }
    }
    else {
        int middle = startPage + left - 1;
        if((page <= middle) && (page >= startPage)) {
            if(!firstHalf) firstHalf = new SoftCopy(ISBN, price, startPage, middle);
            firstHalf->UploadPage(content, page);
        }
        else {
            if(!secondHalf) secondHalf = new SoftCopy(ISBN, price, middle+1, endPage);
            secondHalf->UploadPage(content, page);
        }
    }
}

/*********************************************************************
*	Display
*
* @return content of the pages in the given interval whose borders are
  defined by the first and second @param.
* If there are missing pages (not constructed yet) in the given interval,
  skip them.
* Also content of the pages with id 'from' and with id 'to' are added to
  output (if those pages exist of course).
* For the output, combine the contents of different pages by pushing
  a newline character '\n'.
* See the pdf for the example and explanation.
*/


string SoftCopy::Display(int from, int to) const {
    if(numberOfPages == 1 && from <= startPage && to >= endPage) return pageContent;
    if (firstHalf && secondHalf) {
        string first = firstHalf->Display(from, to);
        string second = secondHalf->Display(from, to);
        if(first.empty()) return second;
        else if(second.empty()) return first;
        while(!first.empty()) {
            if(first[0] == '\n') first.erase(first.begin());
            else if(first[first.length()-1] == '\n') first.erase(first.end());
            else break;
        }
        while(!second.empty()) {
            if(second[0] == '\n') second.erase(second.begin());
            else if(second[second.length()-1] == '\n') second.erase(second.end());
            else break;
        }
        return first + "\n" + second;
    }
    else if (firstHalf) return firstHalf->Display(from, to);
    else if (secondHalf) return secondHalf->Display(from, to);
    else return "";
}

/*********************************************************************
********************* DO NOT TOUCH THE BELOW *************************
/*********************************************************************
*
* Please do NOT do anything about the below methods.
* Please do NOT touch their declarations.
* Do NOT implement them. They will implemented by the admin of the HW.
*/

/*********************************************************************/
