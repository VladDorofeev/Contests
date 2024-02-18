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
    String(const char sym);
    String(const char *line);
    
    void print();

    char* get(); 
    
    void append(const String str);
    int compare(const String str);
    void assign(const String str);
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
}

String::String(const char sym) {
    char *temp = new char[2];
    temp[0] = sym;
    temp[1] = 0;
    this->s = temp;
}

String::String(const char *line) {
    this->s = new char[strlen(line) + 1];
    strcpy(s, line);
}

char *
String::get() {
    return this->s;
}

void
String::print() {
    cout << this->s << endl;
}

void 
String::append(const String str) {
    int line_len = strlen(str.s);
    int this_len = strlen(this->s);

    char *temp = new char[line_len + this_len + 1];

    strcpy(temp, this->s);
    strcat(temp, str.s);

    delete[] this->s;
    this->s = temp;
}

int
String::compare(const String str) {
    return strcmp(this->s, str.s);
}

void
String::assign(const String str) {
    delete[] this->s;
    int len = strlen(str.s);
    this->s = new char[len + 1];
    strcpy(this->s, str.s);
    this->s[len] = 0;
}