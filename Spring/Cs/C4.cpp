#include <iostream>
#include <map>

using std::map;

class Logger
{
public:
    static Logger& getLogger(int key);

    static void set_global_severity(int);
    void set_severity(int);
    int get_severity() const;
    void log(const char *) const;

    ~Logger(){};
private:
    Logger(int);
    Logger(const Logger&);
    Logger operator=(const Logger&);

    static int global_severity;
    int cur_severity;
    int key;


    static map<int, Logger> loggers;

};

int Logger::global_severity = 0;
std::map<int, Logger> Logger::loggers;

Logger::Logger(int _key):cur_severity(0),key(_key) {};
Logger 
Logger::operator=(const Logger& logger) {
    this->cur_severity = logger.cur_severity;
    this->key = logger.key;
    return logger;
}
Logger::Logger(const Logger& logger) {
    this->key = logger.key;
    this->cur_severity = logger.key;
}



void 
Logger::set_global_severity(int gl_sev) {/////////////////////need  rewrite

}

Logger& 
Logger::getLogger(int _key) {
    if (loggers.find(_key) == loggers.end()) {
        loggers[_key] = Logger(_key);
    }
    return loggers[_key];
}



//#ifdef _main

int
main() {
    return 0;
}

//#endif

