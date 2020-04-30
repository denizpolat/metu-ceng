#include "Bank.h"


Bank::Bank() {
    this->_bank_name = "not_defined";
    this->_user_count = 0;
    this->_users = nullptr;
}

Bank::Bank(std::string bank_name, Account* const users, int user_count) {
    this->_bank_name = bank_name;
    this->_user_count = user_count;
    this->_users = new Account[this->_user_count];
    for(int i=0; i<user_count; i++) {
        this->_users[i] = users[i];
    }
}

Bank::~Bank() {
    delete [] _users;
}

Bank::Bank(const Bank& rhs) {
    this->_bank_name = rhs._bank_name;
    this->_user_count = rhs._user_count;
    this->_users = new Account[this->_user_count];
    for(int i=0; i<_user_count; i++) {
        this->_users[i] = rhs._users[i];
    }
}

Bank& Bank::operator+=(const Account& new_acc) {
    for(int i=0; i<this->_user_count; i++) {
        if(this->_users[i] == new_acc) {
            this->_users[i] += new_acc;
            return *this;
        }
    }
    Account* temp = new Account[this->_user_count+1];
    for(int i=0; i<_user_count; i++) {
        temp[i] = this->_users[i];
    }
    temp[_user_count] = new_acc;
    delete [] this->_users;
    this->_users = temp;
    this->_user_count++;
    temp = nullptr;
    return *this;
}

Bank& Bank::operator+=(const Bank& rhs) {
    for(int i=0; i<rhs._user_count; i++) {
        *this += rhs._users[i];
    }
    return *this;
}

Account& Bank::operator[](int account_id) {
    for(int i=0; i<_user_count; i++) {
        if(_users[i] == account_id) return _users[i];
    }
    return _users[0];
}

time_t dateMaker(int month) {
    struct tm some_date = {0};
    int howManyDays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    some_date.tm_hour = 0;
    some_date.tm_min = 0;
    some_date.tm_sec = 0 ;
    some_date.tm_year = (month < 12) ? 119 : 120;
    some_date.tm_mon = month%12;
    some_date.tm_mday = 0;
    time_t some_date_time = mktime(&some_date);
    return some_date_time;
}


bool isUserSafe(Account acc) {
    bool flag = false;
    for(int i=0; i<12; i++) {
        int bal = acc.balance(dateMaker(i), dateMaker(i+1));
        if((bal < 0) && flag) return false;
        else if(bal < 0) flag = true;
        else flag = false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& os, const Bank& bank) {
    int eligibleUsers = 0, totalBalance = 0;
    for(int i=0; i<bank._user_count; i++) {
        if(isUserSafe(bank._users[i])) eligibleUsers++;
        totalBalance += (bank._users[i]).balance();
    }
    os << bank._bank_name << "  " << eligibleUsers <<  "    " << totalBalance << "\n";
     return os;
}


