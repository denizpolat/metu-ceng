#include "HardCopy.h"
/* YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. DO NOT MODIFY THE HEADER FILE AS IT WILL BE
REPLACED BY THE ORIGINAL HEADER DURING TESTING. THE REQUIRED
HEADER FILE IS ALREADY INCLUDED FOR YOU ABOVE THIS COMMENT BLOCK.
THE CONSTRUCTOR, DESTRUCTOR and COPY CONSTRUCTOR ARE ALREADY IMPLEMENTED.
START YOUR IMPLEMENTATIONS BELOW THOSE. */


// DONE
HardCopy::HardCopy(int ISBN, double price) : Book(ISBN, price) {

}

// DONE
HardCopy::HardCopy(const HardCopy& hardcopy) {
	this->ISBN = hardcopy.ISBN;
	this->price = hardcopy.price;
	this->next = hardcopy.next;
}

// DONE
HardCopy::~HardCopy() {

}
/*********************************************************************
*	Clone
*
* @return pointer to a copy of this Book object
*/
Book* HardCopy::Clone() const {
    HardCopy* c = new HardCopy(*this);
    return c;
}
/*********************************************************************
*	Discount
*
* Discounts the price of the book.
* It applies 10% of discount. For example:
  - If the price is 100 TL, the new price will be 90 TL.
  - If the price is 60 TL, the new price will 60 * (90/100) = 54 TL.
*/
void HardCopy::Discount() {
    this->price = price * (0.9);
}
