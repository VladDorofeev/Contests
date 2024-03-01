#include <iostream>
#include <map>

class Logger;

class Loggers 
{
public:
    friend Logger;
    static std::map<int, Logger*> lmap;
    ~Loggers();
private:
};

std::map<int, Logger*> Loggers::lmap;

class Logger
{
public:
    friend Loggers;
    static Logger& getLogger(int key);
    static void set_global_severity(int);

    void set_severity(int);
    int get_severity() const;
    void log(const char *) const;

private:
    Logger(int);
    Logger(const Logger&) = delete;
    Logger operator=(const Logger&) = delete;

    static int global_severity;
    int cur_severity;
    int key;

    
    static Loggers loggers;
};

int Logger::global_severity = 0;
Loggers Logger::loggers;

Logger::Logger(int _key):cur_severity(0),key(_key) {};
Loggers::~Loggers() {
    for (auto [key, log]: lmap) {
        delete log;
    }
}

Logger& 
Logger::getLogger(int _key) {
    if (loggers.lmap.find(_key) == loggers.lmap.end()) {
        loggers.lmap[_key] = new Logger(_key);
    }
    return *loggers.lmap[_key];
}

void 
Logger::set_global_severity(int gl_sev) {
    global_severity = gl_sev;
}
void 
Logger::set_severity(int sev) {
    this->cur_severity = sev;
}
int 
Logger::get_severity() const {
    return this->cur_severity;
}
void 
Logger::log(const char *s) const {
    if (this->cur_severity >= global_severity) {
        std::cout << s << std::endl;
    }
}



#ifdef _main
int
main() {
    Logger::set_global_severity(123);

    Logger::getLogger(1).set_severity(124);
    std::cout << Logger::getLogger(1).get_severity() << std::endl;
    Logger::getLogger(1).log("hahahahah");

    Logger::getLogger(1).set_severity(12);

    Logger::getLogger(1).log("bebebebebe");

    return 0;
}

#endif

