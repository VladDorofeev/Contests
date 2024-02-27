#include <iostream>


class DebugPrinter 
{
public: 
    static DebugPrinter *get();
    ~DebugPrinter();

    DebugPrinter &out(int); 
    DebugPrinter &out(char) ; 

    DebugPrinter( DebugPrinter&) = delete;
    DebugPrinter operator=(DebugPrinter &) = delete;
private:
    static  DebugPrinter *ptr;
    DebugPrinter();
};

DebugPrinter *DebugPrinter::ptr = nullptr;

DebugPrinter*
DebugPrinter::get() {
    if (ptr == nullptr) {
        static DebugPrinter dbg;
        ptr = &dbg;
    }
    return ptr;
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
    friend DebugPrinter& out(DebugPrinter&, IntCharPair&);
private:
    int n;
    char c;
};

IntCharPair::IntCharPair(int _n, char _c) {
    n = _n;
    c = _c;
}

DebugPrinter&
out(DebugPrinter &dbg, IntCharPair &pair) {
    dbg.out(pair.c);
    dbg.out(pair.n);
    return dbg;
}

#ifdef _main

int
main() {
    DebugPrinter *dbg = DebugPrinter::get();
    IntCharPair pair(123, 'q');

    out(dbg->out('>'), pair).out('<');
    return 0;
}

#endif