#include <iostream>
#include <cstring>
#include <exception>

enum {TOKEN_LEN = 128};

void fill_by_different_strings(std::string *buf, size_t size, size_t len);

class TokenPool {
public:
    TokenPool(size_t);
    ~TokenPool();

    size_t token_length() const;
    void get_token(char *);
    void return_token(const char *);
private:
    std::string* token_arr;
    bool* empty_indicator_arr;
    size_t _len;
    size_t _size;

};

TokenPool::TokenPool(size_t cnt)
    : token_arr(0)
    , empty_indicator_arr(0) 
    , _len(TOKEN_LEN)
    , _size(cnt)
{
    try {
        token_arr = new std::string[_size];
        empty_indicator_arr = new bool[_size];
        fill_by_different_strings(token_arr, _size, _len);
        for(size_t i = 0; i < _size; ++i) {
            empty_indicator_arr[i] = true;
        }
    } catch(std::exception&) {
        delete[] token_arr;
        delete[] empty_indicator_arr;
        throw;
    }
}


TokenPool::~TokenPool() {
    delete[] token_arr;
    delete[] empty_indicator_arr;
}


size_t TokenPool::token_length() const {
    return _len;
}


void TokenPool::get_token(char *addr) {
    for(size_t i = 0; i < _size; ++i) {
        if (empty_indicator_arr[i]) {
            empty_indicator_arr[i] = false;
            strcpy(addr, token_arr[i].c_str());
            return;
        }
    }
    throw std::out_of_range("Does not exist empty token.");
}


void TokenPool::return_token(const char *addr) {
    for(size_t i = 0; i < _size; ++i) {
        if (!empty_indicator_arr[i] && strcmp(addr, token_arr[i].c_str()) == 0) {
            empty_indicator_arr[i] = true;
            return;
        }
    }
    throw std::logic_error("There is no non-empty with such string.");
}

