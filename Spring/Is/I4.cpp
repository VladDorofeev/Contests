#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class Department
{
};

class Student
{
public:
    enum { MAX_GRADE = 100 };

    Student(std::string name_); 

    std::string const & name() const;
    int grade() const; 
    Department &department() const; 

    void set_grade(int);
    void set_department(Department *);

private:
    Student(const Student&);
    Student& operator=(const Student &);

    std::string _name;
    int _grade;
    Department *_department;
    
    bool is_grade;
    bool is_department; 
};

struct Application
{
    Application();
    Student *student;
    std::vector<Department *> order;
private:
    Application(const Application&);
    Application& operator=(const Application &);
};

namespace no_algorithm
{
void
assign_with_max_grade(std::vector<Application *> & applications)
{
    for (std::vector<Application *>::iterator iter = applications.begin(); iter != applications.end();) {
        if (((*iter)->student->grade() == (*iter)->student->MAX_GRADE) &&
            ((*iter)->order.empty() == false))
        {
            (*iter)->student->set_department((*iter)->order.front());
            iter = applications.erase(iter);
        } else {
            iter++;
        }
    }
}
}

namespace with_algorithm
{

bool can_assign(Application *appl) {
    if ((appl->student->grade() == appl->student->MAX_GRADE) &&
        (appl->order.empty() == false)) 
    {
        appl->student->set_department(appl->order.front());
        return true;
    }
    return false;
}

void
assign_with_max_grade(std::vector<Application *> & applications)
{
    std::vector<Application *>::iterator end_it = std::remove_if(
        applications.begin(), applications.end(), can_assign);
    applications.erase(end_it, applications.end());
}
}

Student::Student(std::string name_) :
    _name(name_),
    _grade(-1),
    _department(0),
    is_grade(false),
    is_department(false) 

{
}

Student::Student(const Student&) {}
Student& Student::operator=(const Student &) {return *this;}

std::string const & Student::name() const {return _name;}
int Student::grade() const {
    if (is_grade == false) {
        throw std::invalid_argument("Grade doesn`t set");
    }
    return _grade;
}

Department &Student::department() const {
    if (is_department == false) {
        throw std::invalid_argument("Department doesn`t set");
    }
    return *_department;
}

void Student::set_grade(int grade_) {
    is_grade = true;
    _grade = grade_;
}
void Student::set_department(Department *department_) {
    is_department = true;
    _department = department_;
}
Application::Application() {}
Application::Application(const Application&) {}
Application& Application::operator=(const Application &) {return *this;}

#ifdef _main
int main() {
    Department d1;
    std::vector<Department *> order = { &d1 };

    Student s1("Vlad");
    Student s2("Serega");
    Student s3("Sasha");

    s1.set_grade(Student::MAX_GRADE);
    s2.set_grade(Student::MAX_GRADE);
    s3.set_grade(Student::MAX_GRADE);

    Application apl1;
    // apl1.order = order;
    apl1.student = &s1;

    Application apl2;
    apl2.order = order;
    apl2.student = &s2;


    Application apl3;
    // apl3.order = order;
    apl3.student = &s3;


    std::vector<Application *> applications = {&apl1, &apl2, &apl3};

    with_algorithm::assign_with_max_grade(applications);


    std::cout << applications.size() << std::endl;
    for (auto apl : applications) {
        std::cout << apl->student->name() << std::endl;
    }

    try
    {
        s1.department();
        s2.department();
        s3.department();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

    return 0;
}
#endif