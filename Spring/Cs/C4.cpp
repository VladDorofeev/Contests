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

    ~Logger();
private:
    Logger(int);
    Logger(const Logger&);
    Logger operator=(const Logger&);

    static int global_severity;
    int cur_severity;
    int key;


    static map<int, Logger*> loggers;

};

int Logger::global_severity = 0;
std::map<int, Logger*> Logger::loggers;

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
Logger::~Logger() {
    delete loggers[this->key]; 
}


Logger& 
Logger::getLogger(int _key) {
    if (loggers.find(_key) == loggers.end()) {
        loggers[_key] = new Logger(_key);
    }
    return *loggers[_key];
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



//#ifdef _main
int
main() {
    Logger::set_global_severity(123);

    Logger::getLogger(1).set_severity(124);
    std::cout << Logger::getLogger(1).get_severity() << std::endl;
    Logger::getLogger(1).log("hahahahah");

    return 0;
}

//#endif

