#include <iostream>
class DebugPrinter;

class IntCharPair
{
public:
    IntCharPair(int, char);
    // not more than one new declaration is allowed here
    friend class DebugPrinter;
private:
    int n;
    char c;
};

class DebugPrinter 
{
public: 
    ~DebugPrinter();

    static DebugPrinter &get();

    DebugPrinter &out(int);
    DebugPrinter &out(char);

    void set(IntCharPair &pair);

    friend DebugPrinter& out(DebugPrinter &dbg, IntCharPair &pair);

private:
    DebugPrinter(const DebugPrinter &dbg) = delete;
    DebugPrinter& operator=(DebugPrinter &) = delete;

    DebugPrinter();
    
    int n;
    char c;

    static int cnt;
};

int DebugPrinter::cnt = 0;

DebugPrinter::DebugPrinter() {
    this->cnt++;
    this->n = 0;
    this->c = 0;
    std::cout << "Debugger is created." << std::endl;
}

DebugPrinter::~DebugPrinter() {
    if (this->cnt == 0) {
        exit(0);
    }
    std::cout << "Debugger is destroyed." << std::endl;
    this->cnt--;
}

DebugPrinter&
DebugPrinter::get() {
    static DebugPrinter dbg = DebugPrinter();
    return dbg;
}


DebugPrinter&
DebugPrinter::out(int num) {
    std::cout << num;
    return *this;
}

DebugPrinter&
DebugPrinter::out(char sym) {
    std::cout << sym;
    return *this;
}


DebugPrinter&
out(DebugPrinter &dbg, IntCharPair &pair) {
    dbg.set(pair);
    dbg.out(dbg.n);
    dbg.out(dbg.c);
    return dbg;
}

IntCharPair::IntCharPair(int _n, char _c) {
    n = _n;
    c = _c;
}

void
DebugPrinter::set(IntCharPair &pair) {
    n = pair.n;
    c = pair.c;
} 


#ifdef _main

int
main() {
    IntCharPair pair(228, 't');
    out(DebugPrinter::get().out('q'), pair).out(123).out('\n');

    return 0;
}

#endif