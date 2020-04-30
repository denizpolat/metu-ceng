#include "Account.h"

void sort(Transaction* month, int freq) {
    bool sorted = false;
    Transaction tmp;
    int len = freq;
    while (!(sorted) && (len > 1)) {
        sorted = true;
        for (int i=1; i<len; i++) {
            if (month[i-1]>month[i]) {
                tmp = month[i-1];
                month[i-1] = month[i];
                month[i] = tmp;
                sorted = false;
            }
        }
        len--;
    }
}

Account::Account() {
    _id = -1;
    _activity = nullptr;
    _monthly_activity_frequency = nullptr;
}

Account::Account(int id, Transaction** const activity, int* monthly_activity_frequency) {
    this->_id = id;
    int freq=0;
    /*if(monthly_activity_frequency == nullptr) {
        this->_monthly_activity_frequency = nullptr;
        this->_activity = nullptr;
        return;
    }*/
    this->_monthly_activity_frequency = new int [12];
    this->_activity = new Transaction* [12];
    for(int i=0; i < 12; i++) {
        freq = monthly_activity_frequency[i];
        _monthly_activity_frequency[i] = freq;
        sort(activity[i], freq);
        _activity[i] = new Transaction[freq];
        for (int j=0; j<freq; j++) {
            this->_activity[i][j] = activity[i][j];
        }
    }
}

Account::~Account() {
    if(_activity != nullptr) {
        for(int i=0; i<12; i++) {
            delete [] _activity[i];
        }
    }
    delete [] _activity;
    delete [] _monthly_activity_frequency;
}

Account::Account(const Account& rhs) {
    this->_id = rhs._id;
    int freq = 0;
    this->_monthly_activity_frequency = new int[12];
    this->_activity = new Transaction *[12];
    if(rhs._monthly_activity_frequency == nullptr) return;
    for (int i = 0; i < 12; i++) {
        freq = rhs._monthly_activity_frequency[i];
        _monthly_activity_frequency[i] = freq;
        sort(rhs._activity[i], freq);
        _activity[i] = new Transaction[freq];
        for (int j = 0; j < freq; j++) {
            this->_activity[i][j] = rhs._activity[i][j];
        }
    }
}

int findNewFreq(Transaction* month, time_t start, time_t end, int freq) {
    int newfreq = 0;
    if (!freq) return 0;
    for (int j=0; j<freq; j++) {
        if((month[j] > start) && (month[j] < end)) {
            newfreq++;
        }
    }
    return newfreq;
}

Account::Account(const Account& rhs, time_t start_date, time_t end_date) {
    _id = rhs._id;
    int freq=0, index=0, currfreq=0;
    this->_monthly_activity_frequency = new int [12];
    this->_activity = new Transaction* [12];
    for(int i=0; i<12; i++) {
        index = 0;
        currfreq = rhs._monthly_activity_frequency[i];
        freq = findNewFreq(rhs._activity[i], start_date, end_date, currfreq);
        this->_monthly_activity_frequency[i] = freq;
        if(freq) this->_activity[i] = new Transaction[freq];
        else this->_activity[i] = nullptr;
        for (int j=0; j<currfreq; j++) {
            if((rhs._activity[i][j] > start_date) && (rhs._activity[i][j] < end_date)) {
                this->_activity[i][index++] = rhs._activity[i][j];
            }
        }
    }
}

Account::Account(Account&& rhs) {
    _id = rhs._id;
    rhs._id = 0;
    _activity = rhs._activity;
    _monthly_activity_frequency = rhs._monthly_activity_frequency;
    rhs._activity = nullptr;
    rhs._monthly_activity_frequency = nullptr;
}

Account& Account::operator=(Account&& rhs) {
    if ((&rhs != this) && (rhs._activity == nullptr)) {
        for(int i=0; i<12; i++) {
            delete [] _activity[i];
        }
        delete [] _activity;
        delete [] _monthly_activity_frequency;
        this->_id = rhs._id;
        this->_monthly_activity_frequency = rhs._monthly_activity_frequency;
        this->_activity = rhs._activity;
        rhs._activity = nullptr;
        rhs._monthly_activity_frequency = nullptr;
    }
    return *this;
}


Account& Account::operator=(const Account& rhs) {
    if(rhs._monthly_activity_frequency == nullptr) {
        this->_monthly_activity_frequency = nullptr;
        this->_activity = nullptr;
        return *this;
    }
    if (_activity != nullptr) {
        for (int i = 0; i < 12; i++) {
            delete[] _activity[i];
        }
    }
        delete []  _activity;
        delete []  _monthly_activity_frequency;
        this->_id = rhs._id;
        int freq=0;

        this->_monthly_activity_frequency = new int [12];
        this->_activity = new Transaction* [12];
        for(int i=0; i < 12; i++) {
            freq = rhs._monthly_activity_frequency[i];
            _monthly_activity_frequency[i] = freq;
            sort(rhs._activity[i], freq);
            _activity[i] = new Transaction[freq];
            for (int j=0; j<freq; j++) {
                this->_activity[i][j] = rhs._activity[i][j];
            }
        }

    return *this;
}

bool Account::operator==(const Account& rhs) const {
    return (_id == rhs._id);
}

bool Account::operator==(int id) const {
    return (_id == id);
}

Account& Account::operator+=(const Account& rhs) {
    int counter = 0;
    Transaction** list = new Transaction* [12];
    if(rhs._activity == nullptr) return *this;
    for(int i=0; i<12; i++) {
        int tmp_this = this->_monthly_activity_frequency[i];
        int tmp_rhs = rhs._monthly_activity_frequency[i];
        list[i] = new Transaction[tmp_this+tmp_rhs];
        for(int j=0; j<tmp_this; j++) {
            list[i][counter++] = this->_activity[i][j];
        }
        for(int j=0; j<tmp_rhs; j++) {
            list[i][counter++] = rhs._activity[i][j];
        }
        counter = 0;
        sort(list[i],tmp_rhs+tmp_this);
        delete [] _activity[i];
        this->_monthly_activity_frequency[i] = tmp_this+tmp_rhs;
    }
    delete [] this->_activity;
    this->_activity = list;
    list = nullptr;
    return *this;
}

double Account::balance() {
    double result = 0;
    int freq = 0;
    if(this->_activity == nullptr) return 0;
    for(int i=0; i<12; i++) {
        if(!_monthly_activity_frequency[i]) continue;
        freq = _monthly_activity_frequency[i];
        for(int j=0; j<freq; j++) {
            result += (_activity[i][j] + _activity[i][j]);
        }
    }
    result /= 2;
    return result;
}

double Account::balance(time_t end_date) {
    double result = 0;
    Transaction curr = _activity[0][0];
    int j, freq = 0;
    if(this->_activity == nullptr) return 0;
    for(int i=0; i<12; i++) {
        j = 0;
        if(!_monthly_activity_frequency[i]) continue;
        freq = findNewFreq(_activity[i], -1, end_date, _monthly_activity_frequency[i]);
        while((curr < end_date) && (j < freq)) {
            curr = _activity[i][j++];
            result += (curr + curr);
        }
    }
    result /= 2;
    return result;
}

double Account::balance(time_t start_date, time_t end_date) {
    Account a(*this, start_date, end_date);
    double res = a.balance();
    return res;
}



std::ostream& operator<<(std::ostream& os, const Account& account) {
    int currfreq=0;
    if((account._activity == nullptr) || (account._monthly_activity_frequency == nullptr)) os << "-1 \n";
    else {
        os << account._id <<"\n";
        for (int i=0; i<12; i++) {
            currfreq = account._monthly_activity_frequency[i];
            for(int j=0; j<currfreq; j++) {
                os << account._activity[i][j];
            }
        }
    }
    return os;
}
