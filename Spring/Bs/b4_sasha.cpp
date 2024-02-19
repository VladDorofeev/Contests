#include <iostream>
#include <cstring>


class String {
    private:
        char * str_pt;
        int len;
    public:
        String();
        String(const char);
        String(const char *);
        String(const String&);
        ~String();
        char * get();
        void append(const String);
        int compare(const String);
        void assign(const String);
};

String::String() {
    this->str_pt = new char [1];
    this->str_pt[0] = '\0';
    this->len = 0;
}

String::String(const char sym) {
    if (sym == '\0') {
        len = 0;
    } else {
        len = 1;
    }
    this->str_pt = new char[2];
    this->str_pt[0] = sym;
    this->str_pt[1] = '\0';
}

String::String(const char* c_str) {
    this->len = strlen(c_str);
    this->str_pt = new char[this->len + 1];
    strcpy(this->str_pt, c_str);
}

String::String(const String& str) {
    this->len = str.len;
    this->str_pt = new char[this->len + 1];
    strcpy(this->str_pt, str.str_pt);
}

String::~String() {
    delete[] this->str_pt; 
}

char * String::get() {
    return this->str_pt;
}
void String::append(const String elem) {
    char* new_str = new char[this->len + elem.len + 1];
    strcat(new_str, this->str_pt);
    strcat(new_str, elem.str_pt);
    delete[] this->str_pt;
    this->len = this->len + elem.len;
    this->str_pt = new_str;
}

int String::compare(const String elem) {
    return strcmp(this->str_pt, elem.str_pt);
}

void String::assign(const String elem) {
    char * new_str = new char[elem.len + 1];
    strcpy(new_str, elem.str_pt);
    delete[] this->str_pt;
    this->str_pt = new_str;
    this->len = elem.len;
}

int
main() {
    String s;
    s.assign("sddww");
    String a;
    a.assign('a');
    a.append(s);
    std::cout << a.get() << std::endl;
}