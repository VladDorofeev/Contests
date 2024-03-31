#include <iostream>
#include <string>
#include <cstring>

void fill_by_different_strings(std::string *buf, size_t size, size_t len);

class TokenPool
{
public:
    TokenPool(size_t);
    ~TokenPool();

    size_t token_length() const;
    void get_token(char *addr);
    void return_token(const char *addr);
private:
    std::string *arr;
    bool *indicators;

    size_t sz;
    size_t len;

    enum {LEN = 86};
};

TokenPool::TokenPool(size_t _sz) :
    arr(nullptr),
    indicators(nullptr),
    sz(_sz),
    len(LEN)
{
    try
    {
        arr = new std::string[sz];
        indicators = new bool[sz];
        fill_by_different_strings(arr, sz, len);
    }
    catch(const std::exception& e)
    {
        delete[] arr;
        delete[] indicators;
        throw;
    }
    
    for (size_t i = 0; i < sz; i++) {
        indicators[i] = true;
    }
}

TokenPool::~TokenPool() {
    delete[] arr;
    delete[] indicators;
}

size_t TokenPool::token_length() const {return len;}

void TokenPool::get_token(char *addr) {
    for (size_t i = 0; i < sz; i++) {
        if (indicators[i] == true) {
            indicators[i] = false;
            strcpy(addr, arr[i].c_str());
            return;
        }
    }
    throw std::runtime_error("No free tokens");
}

void TokenPool::return_token(const char *addr) {
    for (size_t i = 0; i < sz; i++) {
        if ((strcmp(addr, arr[i].c_str()) == 0) && (indicators[i] == false)) {
            indicators[i] = true;
            return;
        }
    }
    throw std::runtime_error("No match busy token for return");
}

int main() {
    return 0;
}