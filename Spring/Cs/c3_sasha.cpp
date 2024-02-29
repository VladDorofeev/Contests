#include <iostream>

class DebugPrinter {
private:
    DebugPrinter();
    DebugPrinter(const DebugPrinter&);
    ~DebugPrinter();
public:
    static DebugPrinter& get();
    DebugPrinter& out(const int&);
    DebugPrinter& out(const char&);
};

class IntCharPair {
public:
    IntCharPair(int, char);
    friend DebugPrinter& out(const DebugPrinter&, const IntCharPair&);
    // not more than one new declaration is allowed here
private:
    int n;
    char c;
};

DebugPrinter::DebugPrinter() {
    std::cout << "Debugger is created.\n";
}

DebugPrinter::~DebugPrinter() {
    std::cout << "Debugger is destroyed.\n";
}

DebugPrinter& DebugPrinter::get() {
    static DebugPrinter solo;
    return solo;
}

DebugPrinter& DebugPrinter::out(const int& num) {
    std::cout << num;
    return *this;
}

DebugPrinter& DebugPrinter::out(const char& sym) {
    std::cout << sym;
    return *this;
}

IntCharPair::IntCharPair(int num, char sym) {
    n = num;
    c = sym;
}

DebugPrinter& out(const DebugPrinter& deb, const IntCharPair& pair) {
    return DebugPrinter::get().out('(').out(pair.n).out(',').out(' ').out(pair.c).out(')');
}