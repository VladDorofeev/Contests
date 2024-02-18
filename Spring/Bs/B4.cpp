#include <iostream>
#include <cstring>

using std::cout;
using std::endl;

class String 
{
    char *s;
public:
    String();
    ~String();
    String(const String& str);
    
    char *cast(const char sym);

    void print();

    char* get(); 
    
    void append(const char sym);
    void append(const char *line);

    int compare(const char sym);
    int compare(const char *line);

    void assign(const char sym);
    void assign(const char *line);

    operator char*() const;

};


String::String() {
    this->s = new char[1];
    *s = 0;
}

String::~String() {
    delete[] this->s;
}

String::String(const String& str) {
    this->s = new char[strlen(str.s) + 1];
    strcpy(this->s, str.s);
    this->s[strlen(str.s)] = 0;
}

char *
String::get() {
    return this->s;
}

void
String::append(const char sym) {
    char *temp = this->cast(sym);
    this->append(temp);
    delete[] temp;
}

void 
String::append(const char *line) {
    int line_len = strlen(line);
    int this_len = strlen(this->s);

    char *temp = new char[line_len + this_len + 1];

    for (int i = 0; i < this_len; i++) {
        temp[i] = this->s[i];
    }
    for (int i = this_len; i < this_len + line_len + 1; i++) {
        temp[i] = line[i - this_len];
    }
    temp[line_len + this_len] = 0;

    delete[] this->s;
    this->s = temp;
}


void
String::print() {
    cout << this->s << endl;
}

int
String::compare(const char sym) {
    char *temp = this->cast(sym);
    int res = this->compare(temp);
    delete[] temp;
    return res;
}

int
String::compare(const char *line) {
    return strcmp(this->s, line);
}


void
String::assign(const char sym) {
    char *temp = this->cast(sym);
    this->assign(temp);
    delete[] temp;
}

void
String::assign(const char *line) {
    delete[] this->s;
    int len = strlen(line);
    this->s = new char[len + 1];
    strcpy(this->s, line);
    this->s[len] = 0;
}


char *
String::cast(const char sym) {
    char *temp = new char[2];
    temp[0] = sym;
    temp[1] = 0;
    return temp;
}

String::operator char*() const {
    return this->s;
}
