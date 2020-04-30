#include "Transaction.h"


Transaction::Transaction() {
    _amount = -1;
    _date = -1;
}

Transaction::Transaction(double amount, time_t date) {
    _amount = amount;
    _date = date;
}

Transaction::Transaction(const Transaction& rhs) {
    _amount = rhs._amount;
    _date = rhs._date;
}

bool Transaction::operator<(const Transaction& rhs) const {
    int diff = std::difftime(this->_date, rhs._date);
    return diff < 0;
}

bool Transaction::operator>(const Transaction& rhs) const {
    int diff = std::difftime(this->_date, rhs._date);
        return diff > 0;
    }

bool Transaction::operator<(const time_t date) const {
    int diff = std::difftime(this->_date, date);
        return diff < 0;
    }

bool Transaction::operator>(const time_t date) const {
    int diff = std::difftime(this->_date, date);
    return diff > 0;
}

double Transaction::operator+(const Transaction& rhs) {
    return this->_amount + rhs._amount;
}

double Transaction::operator+(const double add) {
    return this->_amount + add;
}

Transaction& Transaction::operator=(const Transaction& rhs) {
    this->_amount = rhs._amount;
    this->_date = rhs._date;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Transaction& transaction) {
    tm* tmp = std::localtime(&(transaction._date));
    os << transaction._amount << "  -   " << tmp->tm_hour <<":" << tmp->tm_min << ":" << tmp->tm_sec << "-" << tmp->tm_mday << "/" << tmp->tm_mon + 1 << "/" << tmp->tm_year + 1900 << "\n";
    return os;
}

