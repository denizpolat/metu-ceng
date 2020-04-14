#include "hw3.h"



// ACTIVE USERS FUNCTIONS


ActiveUsers::ActiveUsers() {};


ActiveUsers::ActiveUsers(int table_size) {
	ActiveTable.resize(table_size);
	this->ActiveTableSize = table_size;
	for (int i = 0; i < table_size; i++) {
		ActiveTable[i] = EMPTY;
	}
}



ActiveUsers::~ActiveUsers() {
	ActiveTable.clear();
}


bool ActiveUsers::isIn(std::string username) {
	for (int i = 0; i < ActiveTable.size(); i++) {
		if (ActiveTable[hashFunction(username, ActiveTable.size(), i)] == username) return true;
		else continue;
	}
	return false;
}


void ActiveUsers::logOutActive(std::string user) {
	int size = ActiveTable.size();
	for (int i = 0; i < size; i++) {
		if (ActiveTable[hashFunction(user, size, i)] == user) {
			ActiveTable[hashFunction(user, size, i)] = DELETED;
		}
	}
}


int ActiveUsers::addActive(std::string username) {
	for (int i = 0; i < ActiveTableSize; i++) {
		int index = hashFunction(username, ActiveTableSize, i);
		if (ActiveTable[index] == EMPTY || ActiveTable[index] == DELETED) {
			ActiveTable[index] = username;

			if (loadFactor() > MAX_LOAD_FACTOR) {
				rehash();
			}
			return 1;
		}
	}
	return 0;
}






float ActiveUsers::printUser() {
	int count = 0;
	for (int i = 0; i < ActiveTableSize; i++) {
		std::cout << ActiveTable[i] << std::endl;
		count++;
	}
	return ((float)count) / ActiveTableSize;
}


bool ActiveUsers::isPrime(int number) {
	int sqroot = sqrt(number);
	for (int i = 2; i <= sqroot; i++) {
		if (number % i == 0) return false;
	}
	return true;
}



int ActiveUsers::findPrime(int old) {
	int newVal = old * 2 + 1;
	while (!isPrime(newVal)) {
		newVal++;
	}
	return newVal;
}


void ActiveUsers::resize(int curr_size) {
	ActiveTable.resize(curr_size);
	this->ActiveTableSize = curr_size;
	for (int i = 0; i < curr_size; i++) {
		ActiveTable[i] = EMPTY;
	}
}




void ActiveUsers::rehash() {
	std::vector<std::string> toDoList;
	int toDoSize = 0;
	for (int i = 0; i < ActiveTableSize; i++) {
		if (ActiveTable[i] != EMPTY && ActiveTable[i] != DELETED) {
			toDoList.push_back(ActiveTable[i]);
			toDoSize++;
		}
	}
	resize(findPrime(ActiveTableSize));

	for (int j = 0; j < toDoSize; j++) {
		addActive(toDoList[j]);
	}
}



float ActiveUsers::loadFactor() {
	int count = 0;
	for (int i = 0; i < ActiveTableSize; i++) {
		std::string user = ActiveTable[i];
		if (user != EMPTY && user != DELETED) {
			count++;
		}
	}
	return ((float)count) / ActiveTableSize;
}





//ALL USERS FUNCIONS


AllUsers::AllUsers() {};


AllUsers::AllUsers(int table_size) {
	AllTable.resize(table_size);
	this->AllTableSize = table_size;
	for (int i = 0; i < table_size; i++) {
		AllTable[i].first = EMPTY;
		AllTable[i].second = EMPTY;
	}
}


AllUsers::~AllUsers() {
	AllTable.clear();
}


void AllUsers::findAllPasswords(std::string user, std::vector <std::pair<std::string, std::string> > &allPasswords) {
	for (int i = 0; i < AllTableSize; i++) {
		int index = hashFunction(user, AllTableSize, i);
		if (AllTable[index].first == user) {
			allPasswords.push_back(AllTable[index]);
		}
	}
}


void AllUsers::Resize(int table_size) {
	AllTable.resize(table_size);
	this->AllTableSize = table_size;
	for (int i = 0; i < table_size; i++) {
		AllTable[i].first = EMPTY;
		AllTable[i].second = EMPTY;
	}
}


void AllUsers::rehash() {
	std::vector <std::pair<std::string, std::string> > toBeInserted;
	for (int a = 0; a < AllTableSize; a++) {
		int check = 0;
		if (AllTable[a].first != EMPTY && AllTable[a].first != DELETED) {

			bool flag = true;
			while (check < toBeInserted.size()) {
				if (toBeInserted[check].first == AllTable[a].first) {
					flag = false;
				}
				check++;
			}
			if (flag == true) {
				toBeInserted.push_back(AllTable[a]);
			}
		}
	}
	std::vector <std::pair<std::string, std::string > > passwords;


	/*tobeinsertedi loopla her eleman icin allpasswordsa ekle*/
	for (int d = 0; d < toBeInserted.size(); d++) {
		if (toBeInserted[d].first != EMPTY && toBeInserted[d].first != DELETED) {
			findAllPasswords(toBeInserted[d].first, passwords);
		}
	}


	Resize(findPrime(AllTableSize));

	//  iterate allpasswords insert to table
	for (int z = 0; z < passwords.size(); z++) {
		AddToAll(passwords[z].first, passwords[z].second);
	}
}


int AllUsers::AddToAll(std::string username, std::string password) {
	for (int i = 0; i < AllTableSize; i++) {
		int index = hashFunction(username, AllTableSize, i);
		if (AllTable[index].first == EMPTY || AllTable[index].first == DELETED) {
			AllTable[index].first = username;
			AllTable[index].second = password;

			if (loadFactor() > MAX_LOAD_FACTOR) {
				rehash();
			}
			return 1;
		}
	}
	return 0;
}


bool AllUsers::isPrime(int num) {
	int sqroot = (int)sqrt(num);
	for (int i = 2; i <= sqroot; i++) {
		if (num % i == 0) {
			return false;
		}
	}
	return true;
}


int AllUsers::findPrime(int oldVal) {
	int newVal = oldVal * 2 + 1;
	while (!isPrime(newVal)) {
		newVal++;
	}
	return newVal;
}



float AllUsers::loadFactor() {
	int count = 0;
	int size = AllTable.size();
	for (int i = 0; i < size; i++) {
		std::string username = AllTable[i].first;
		if (username != EMPTY && username != DELETED) {
			count++;
		}
	}
	return ((float)count) / size;
}



int AllUsers::deleteAll(std::string user, std::vector<std::string> &oldPsswrds) {
	for (int i = 0; i < AllTable.size(); i++) {
		if (AllTable[hashFunction(user, AllTable.size(), i)].first == user) {
			oldPsswrds.push_back(AllTable[hashFunction(user, AllTable.size(), i)].second);
			AllTable[hashFunction(user, AllTable.size(), i)].first = DELETED;
			AllTable[hashFunction(user, AllTable.size(), i)].second = DELETED;
		}
	}
	if (oldPsswrds.empty()) return 0;
	else return 1;
}



bool AllUsers::isIn(std::string username) {
	for (int i = 0; i < AllTableSize; i++) {
		if (AllTable[hashFunction(username, AllTableSize, i)].first == username) return true;
		else continue;
	}
	return false;
}


bool AllUsers::isIn(std::string username, std::string password) {
	for (int i = 0; i < AllTableSize; i++) {
		if (AllTable[hashFunction(username, AllTableSize, i)].first == username && AllTable[hashFunction(username, AllTableSize, i)].second == password) return true;
		else continue;
	}
	return false;
}


int AllUsers::findAllIndex(std::string username, std::string password) {
	int index = 0;
	for (int i = 0; i < AllTable.size(); i++) {
		if (AllTable[hashFunction(username, AllTable.size(), i)].first == username && AllTable[hashFunction(username, AllTable.size(), i)].second == password) {
			index = hashFunction(username, AllTable.size(), i);
		}
	}
	return index;
}


std::string AllUsers::findLast(std::string username) {
	std::string last = "";
	for (int i = 0; i < AllTable.size(); i++) {
		if (AllTable[hashFunction(username, AllTable.size(), i)].first == username) {
			last = AllTable[hashFunction(username, AllTable.size(), i)].second;
		}
	}
	return last;
}




float AllUsers::printPassword() {
	int size = AllTable.size();
	int count = 0;
	for (int i = 0; i < size; i++) {
		std::cout << AllTable[i].first << " " << AllTable[i].second << std::endl;
		count++;
	}
	return ((float)count) / size;
}



int AllUsers::fromFile(std::string fileName) {
	int count = 0;
	std::ifstream file(fileName.c_str());
	std::string deliminator = " ";
	std::string here;
	// ifstream file;
		// file.open(fileName.c_str);
	while (getline(file, here)) {
		std::string usrnm, psswrd;
		int indexToDivide;
		if (here == "") break;
		indexToDivide = here.find(deliminator, 0);
		usrnm = here.substr(0, indexToDivide);
		psswrd = here.substr(indexToDivide + 1);
		if (!isIn(usrnm)) {
			AddToAll(usrnm, psswrd);
			count++;
		}

	}
	return count;
}



//GIVEN


AccessControl::AccessControl(int table1Size, int table2Size) {
	regUsers = AllUsers(table1Size);
	activeUsers = ActiveUsers(table2Size);
}


AccessControl::~AccessControl() {
	;
}


int AccessControl::addUser(std::string username, std::string pass) {
	if (regUsers.isIn(username)) {
		return 0;
	}
	else {
		regUsers.AddToAll(username, pass);
		return 1;
	}
}


int AccessControl::addUsers(std::string filePath) {
	return regUsers.fromFile(filePath);
}


int AccessControl::delUser(std::string username, std::vector <std::string> &oldPasswords) {
	return regUsers.deleteAll(username, oldPasswords);
}



int AccessControl::changePass(std::string username, std::string oldpass, std::string newpass) {
	if (regUsers.isIn(username, oldpass)) {
		if (regUsers.findLast(username) == oldpass) {
			regUsers.AddToAll(username, newpass);
			return 1;
		}
	}
	return 0;
}


int AccessControl::login(std::string username, std::string pass) {
	if (regUsers.findLast(username) == pass && !(activeUsers.isIn(username))) {
		return activeUsers.addActive(username);
	}
	else return 0;
}


int AccessControl::logout(std::string username) {
	if (activeUsers.isIn(username)) {
		activeUsers.logOutActive(username);
		return 1;
	}
	return 0;
}


float AccessControl::printActiveUsers() {
	return activeUsers.printUser();
}


float AccessControl::printPasswords() {
	return regUsers.printPassword();
}

