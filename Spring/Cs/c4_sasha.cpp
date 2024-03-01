#include <iostream>

class Logger {
private:
    struct l_key {
        int key;
        Logger* pt_lg;
    };
    class Lk_mas{
    public:
    	~Lk_mas();
        Lk_mas();
        void pushback(int, Logger*);
        l_key* mas;
        int size;
        int capacity;
    };
    int severity;
    Logger();
    Logger(const Logger&);
    static Lk_mas pt_lk_mas;
    static int global_severity;
public:
    static Logger& getLogger(const int&);
    static void set_global_severity(int);
    void set_severity(int); 
    int get_severity() const;
    void log(const char *) const;
};

Logger::Logger() {
    severity = 0;
}

void Logger::set_global_severity(int sev) {
    global_severity = sev;
}

void Logger::set_severity(int sev) {
    severity = sev;
}

int Logger::get_severity() const {
    return severity;
}

void Logger::log(const char * msg) const {
    if (severity >= global_severity) {
        std::cout << msg << std::endl;
    }
}

Logger::Lk_mas::Lk_mas() {
    mas = nullptr;
    size = 0;
    capacity = 0;
}

Logger::Lk_mas::~Lk_mas() {
    for(int i = 0; i < size; ++i) {
        delete mas[i].pt_lg;
    }
    delete[] mas;
}

void Logger::Lk_mas::pushback(int k, Logger* l_pt) {
    if (capacity == size) {
        l_key* new_mas = nullptr;
        if (capacity == 0) {
            capacity = 100;
        } else {
            capacity = 2 * capacity;
        }
        new_mas = new l_key [capacity];
        for(int i = 0; i < size; ++i) {
            new_mas[i] = mas[i];
        }
        delete[] mas;
        mas = new_mas;
    }
    l_key new_elem = {k, l_pt};
    mas[size++] = new_elem;
}

Logger& Logger::getLogger(const int& k) {
    for (int i = 0; i < pt_lk_mas.size; ++i) {
        if (pt_lk_mas.mas[i].key == k) {
            return *(pt_lk_mas.mas[i].pt_lg);
        }
    }
    Logger* l_pt = nullptr;
    l_pt = new Logger;
    pt_lk_mas.pushback(k, l_pt);
    return *l_pt;
}

Logger::Lk_mas  Logger::pt_lk_mas;
int Logger::global_severity = 0;
