#ifndef _HW3_H
#define _HW3_H
#include <iostream>
#include <string>
#include <vector>
#include <utility>  // use just for std::pair< >
#include <fstream>
#include <math.h>

#define MAX_LOAD_FACTOR 0.65
#define EMPTY "EMPTY"
#define DELETED "DELETED"




class AllUsers {
public:
	AllUsers();
	AllUsers(int table_size);
	~AllUsers();
	bool isPrime(int num);
	int findPrime(int oldVal);
	int AddToAll(std::string username, std::string password);
	void Resize(int table_size);
	int deleteAll(std::string user, std::vector<std::string> &oldPsswrds);
	bool isIn(std::string user, std::string password);
	bool isIn(std::string user);
	std::string findLast(std::string username);
	int findAllIndex(std::string username, std::string password);
	float printPassword();
	int fromFile(std::string fileName);


private:

	std::vector <std::pair<std::string, std::string> > AllTable;
	int AllTableSize;
	void findAllPasswords(std::string user, std::vector <std::pair<std::string, std::string> > &allPasswords);
	void rehash();
	float loadFactor();


	int hashFunction(std::string key, int tableSize, int i) {
		int length = key.length();
		int newK = 0;
		for (int j = 0; j < length; j++) {
			newK += (int)key[j];
		}
		return ((newK % tableSize) +
			i * ((newK
				* tableSize - 1) % tableSize)) %
			tableSize;
	}

};

class ActiveUsers {
public:
	ActiveUsers();
	ActiveUsers(int table_size);
	~ActiveUsers();
	void logOutActive(std::string user);
	int addActive(std::string username);
	bool isIn(std::string username);
	float printUser();
	bool isPrime(int number);
	int findPrime(int old);
	void resize(int curr_size);
	void rehash();





private:

	std::vector <std::string> ActiveTable;
	int ActiveTableSize;
	float loadFactor();


	int hashFunction(std::string key, int tableSize, int i) {
		int length = key.length();
		int newK = 0;
		for (int i = 0; i < length; i++) {
			newK += (int)key[i];
		}
		return ((newK % tableSize) +
			i * ((newK
				* tableSize - 1) % tableSize)) %
			tableSize;
	}

};
class AccessControl {
public:
	AccessControl(int table1Size, int table2Size);
	~AccessControl();

	int addUser(std::string username, std::string pass);
	int addUsers(std::string filePath);
	int delUser(std::string username, std::vector <std::string> &oldPasswords);
	int changePass(std::string username, std::string oldpass, std::string newpass);

	int login(std::string username, std::string pass);
	int logout(std::string username);

	float printActiveUsers();
	float printPasswords();

private:
	//<<your hash class>> activeUsers;	// hash(username) -> username
	ActiveUsers activeUsers;

	//<<your hash class>> regUsers;  		// hash(username) -> username,password
	AllUsers regUsers;


	int hashFunction(std::string key, int tableSize, int i) {
		int length = key.length();
		int newK = 0;
		for (int j = 0; j < length; j++) {
			newK += (int)key[j];
		}
		// hash function 1 -> (newK % tableSize)
		// hash function 2 -> (newK * tableSize - 1) % tableSize)

		return ((newK % tableSize) +
			i * ((newK
				* tableSize - 1) % tableSize)) %
			tableSize;
	}
};


#endif