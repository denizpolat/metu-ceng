#include "hw3.h"

using namespace std;


void inp3() {
	AccessControl ac(1, 1);
	cout << ac.addUsers("input.txt") << endl;
	ac.printPasswords();
	cout << endl;
}



int main() {
	inp3();
	std::cin.get();
	return 0;
}
