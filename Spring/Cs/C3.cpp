#include <iostream>


class DebugPrinter 
{
public: 
    static DebugPrinter &get();

    DebugPrinter &out(int); 
    DebugPrinter &out(char); 

    DebugPrinter(const DebugPrinter&) = delete;
    DebugPrinter operator=(const DebugPrinter&) = delete;
private:
    ~DebugPrinter();
    DebugPrinter();
};

DebugPrinter&
DebugPrinter::get() {
    static DebugPrinter dbg;
    return dbg;
}

DebugPrinter::~DebugPrinter() {
    std::cout << "Debugger is destroyed." << std::endl;
}

DebugPrinter::DebugPrinter() {
    std::cout << "Debugger is created." << std::endl;
}

DebugPrinter &
DebugPrinter::out(int n)  {
    std::cout << n;
    return *this;
} 

DebugPrinter &
DebugPrinter::out(char c)  {
    std::cout << c;
    return *this;
}

class IntCharPair
{
public:
    IntCharPair(int, char);
    // not more than one new declaration is allowed here
    friend DebugPrinter& out(const DebugPrinter&, const IntCharPair&);
private:
    int n;
    char c;
};

IntCharPair::IntCharPair(int _n, char _c) {
    n = _n;
    c = _c;
}

DebugPrinter&
out(const DebugPrinter &dbg, const IntCharPair &pair) {
    //(n, c)
    return DebugPrinter::get().out('(').out(pair.n).out(',').out(' ').out(pair.c).out(')');
}

#ifdef _main

int
main() {
    IntCharPair pair(123, 'q');
    out(DebugPrinter::get().out('>'), pair).out('<');
    return 0;
}

#endif