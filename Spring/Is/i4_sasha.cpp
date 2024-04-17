#include <iostream>
#include <vector>
#include <exception>
#include <algorithm>

class Department
{
};

// information card about student
class Student
{
public:
    enum { MAX_GRADE = 100 };
    Student(std::string name_); // no necessary copy of std::string
    std::string const & name() const;
    int grade() const; // throws when grade was not set
    Department &department() const; // throw when department was not set

    void set_grade(int);
    void set_department(Department *);
private:
// TODO disable copying in user code
    Student(Student const&);
    Student& operator=(Student const&);
    std::string _name;
    int _grade;
    Department* _department;
// TODO complete the class
};
//Student impl
Student::Student(std::string name_)
    : _name(name_)
    , _grade(-1)
    , _department(0)   
{
}


Student::Student(Student const& other)
    : _name(other._name)
    , _grade(other._grade)
    , _department(other._department)   
{
}

Student& Student::operator=(Student const& other) {
    if (this != &other) {
        _name = other._name;
        _grade = other._grade;
        _department = other._department;
    }
    return *this;
}


std::string const& Student::name() const {
    return _name;
}

int Student::grade() const {
    if (_grade < 0) {
        throw std::exception();
    }
    return _grade;
}


Department & Student::department() const {
    if (!_department) {
        throw std::exception();
    }
    return *_department;
}


void Student::set_grade(int grade_) {
    _grade = grade_;
}


void Student::set_department(Department * department_) {
    _department = department_;
}

struct Application
{
    Application();
    Student *student;
    std::vector<Department *> order; // may be empty

// TODO disable copying in user code
private:
    Application(Application const&);
    Application& operator=(Application const&);
};
//Application impl
Application::Application() 
    : student(0)
    , order()
{
}

Application::Application(Application const& other)
    : student(other.student)
    , order(other.order)
{
}

Application& Application::operator=(Application const& other) {
    if (this != &other){
        student = other.student;
        order = other.order;
    }
    return *this;
}

namespace no_algorithm
{
// students with MAX_GRADE and non empty order
//  are assigned to the department from the first item of its order
void
assign_with_max_grade(std::vector<Application *> & applications)
{
    // TODO just plain loops, no additional functions
    for(std::vector<Application *>::iterator it = applications.begin(); it != applications.end();) {
        if ((*it)->order.size() != 0 && (*it)->student->grade() == Student::MAX_GRADE) {
            (*it)->student->set_department((*it)->order[0]);
            it = applications.erase(it);
        } else {
            ++it;
        }
    }
}
}

namespace with_algorithm
{

// it is possible to add code here
bool predicate(Application* application) {
    if (application->order.size() != 0 && application->student->grade() == Student::MAX_GRADE) {
        application->student->set_department(application->order[0]);
        return true;
    }
    return false;
}
// students with MAX_GRADE and non empty order
//  are assigned to the department from the first item of its order
void
assign_with_max_grade(std::vector<Application *> & applications)
{
    // TODO no loops, no recursion
    std::vector<Application*>::iterator it_end = std::remove_if
        (applications.begin(), applications.end(), predicate);
    applications.erase(it_end, applications.end());
}
}

// main() should be removed/commented
