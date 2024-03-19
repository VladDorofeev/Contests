#include <iostream>
#include <cstring>
//Created 21:12 (18.03)
//0:30 +
//23:11 -- 1:30 == 2:20
//09:15 -- 

class Person
{
public:
    Person(const char *name, int age, bool man);

    const char *name() const;
    int age() const; 
    bool man() const;
private:
    const char *_name;
    int _age;
    bool _man;
};

Person::Person(const char *name, int age, bool man):
    _name(name),
    _age(age),
    _man(man)
{
}

const char *Person::name() const {return _name;}
int Person::age() const {return _age;}
bool Person::man() const {return _man;}



class PersonView
{
public:
    class Iterator
    {
    public:
        Iterator();
        Iterator(bool (*) (const Person&), const Person *, int);

        bool operator!= (const Iterator&) const;
        Iterator& operator++();
        const Person* operator->()const;
    private:
        bool (*selection_bias) (const Person&);
        const Person *begin;
        const Person *ptr;
        int sz;
    };

    PersonView();
    PersonView(bool (*) (const Person&), const Person *, int);

    Iterator begin() const;
    Iterator end() const;
protected:
    bool (*selection_bias) (const Person&);
    const Person *begin_ptr;
    int sz;
};

PersonView::Iterator::Iterator():
    selection_bias(nullptr),
    begin(nullptr),
    ptr(nullptr),
    sz(0)
{
}

PersonView::Iterator::Iterator(bool (*_sel_bias) (const Person&), const Person *_ptr, int _sz):
    selection_bias(_sel_bias),
    begin(_ptr),
    ptr(_ptr),
    sz(_sz)
{
}

bool PersonView::Iterator::operator!= (const Iterator& other) const 
{
    return ptr != other.ptr;
}

PersonView::Iterator& PersonView::Iterator::operator++() {
    if (ptr == begin+sz) {
        return *this;
    }

    for (ptr++; ptr != begin + sz; ptr++) {
        if (selection_bias(*ptr)) {
            break;
        }
    }

    return *this;
}

const Person* PersonView::Iterator::operator->()const {
    return ptr;
}

PersonView::PersonView():
    selection_bias(nullptr),
    begin_ptr(nullptr),
    sz(0)
{
}

PersonView::PersonView(bool (*_sel_bias) (const Person&), const Person *_ptr, int _sz): 
    selection_bias(_sel_bias),
    begin_ptr(_ptr),
    sz(_sz)
{
}

PersonView::Iterator PersonView::begin() const {
    return Iterator(selection_bias, begin_ptr, sz);
}
PersonView::Iterator PersonView::end() const {
    return Iterator(selection_bias, begin_ptr + sz, 0);
}

bool man_func (const Person &obj) {
    return obj.man();
}
bool young_func (const Person &obj) {
    return ((obj.age() >= 18) && ((obj.age() <= 35)));
}

class MenPersonView : public PersonView
{
public:
    MenPersonView(bool (*) (const Person&), const Person *, int sz);
    Iterator begin() const;
};

MenPersonView::MenPersonView(bool (*_sel_bias) (const Person&), const Person *_ptr, int _sz) :
    PersonView(_sel_bias, _ptr, _sz)
{
}

PersonView::Iterator MenPersonView::begin() const {
    const Person *temp = begin_ptr;
    int i = 0;
    for (; i < sz; ++i) {
        if (man_func(*temp)){
            break;
        }
        temp++;
    }
    return Iterator(selection_bias, temp, sz - i);
}

class YoungPersonView : public PersonView
{
public:
    YoungPersonView(bool (*) (const Person&), const Person *, int sz);
    Iterator begin() const;
};

YoungPersonView::YoungPersonView(bool (*_sel_bias) (const Person&), const Person *_ptr, int _sz) :
    PersonView(_sel_bias, _ptr, _sz)
{
}

PersonView::Iterator YoungPersonView::begin() const {
    const Person *temp = begin_ptr;
    int i = 0;
    for (; i < sz; ++i) {
        if (young_func(*temp)){
            break;
        }
        temp++;
    }
    return Iterator(selection_bias, temp, sz - i);
}


class Persons
{
public:
    Persons();
    Persons(const Persons&);
    Persons& operator= (const Persons&);
    ~Persons();

    void add(const Person&);

    MenPersonView men();
    YoungPersonView young();
private:
    int sz;
    int cap;
    Person *persons;

    enum { START_SZ = 100 };
};

Persons::Persons():
    sz(0),
    cap(0),
    persons(nullptr)
{
}

Persons::Persons(const Persons &other):
    sz(other.sz),
    cap(other.cap)
{
    if (other.persons != nullptr) {
        persons = static_cast<Person*>(operator new[] (cap * sizeof(Person)));
        std::memcpy(persons, other.persons, cap * sizeof(Person));
    } else {
        persons = nullptr;
    } 
}

Persons&
Persons::operator=(const Persons &other)
{
    if (this == &other) {
        return *this;
    }

    sz = other.sz;
    cap = other.cap;
    delete[] persons;

    if (other.persons != nullptr) {
        persons = static_cast<Person*>(operator new[](cap * sizeof(Person)));
        std::memcpy(persons, other.persons, cap * sizeof(Person));
    } else {
        persons = nullptr;
    }

    return *this;
}

Persons::~Persons() {
    sz = 0;
    cap = 0;
    delete[] persons;
}


void Persons::add(const Person& person) {
    if (sz == cap) {
        cap = cap == 0 ? START_SZ : cap * 2;
        Person *temp = static_cast<Person*>(operator new[](cap * sizeof(Person)));
        
        if (persons != nullptr) {
            std::memcpy(temp, persons, cap * sizeof(Person));
        }

        delete[] persons;
        persons = temp;
    }
    persons[sz++] = person;
}


MenPersonView Persons::men() 
{
    return MenPersonView(man_func, persons, sz);
}


YoungPersonView Persons::young() 
{
    return YoungPersonView(young_func, persons, sz);
}


#ifdef _main
int
main(void) {
    Persons org;
    org.add(Person("Ivanov", 20, true));
    org.add(Person("Ivanova", 20, true));
    org.add(Person("Petrov", 20, true));
    org.add(Person("Sidorov", 20, true));

    std::cout << "Men:" << std::endl;
    MenPersonView men = org.men();
    for (MenPersonView::Iterator it = men.begin(); it != men.end(); ++it) {
        std::cout << it->name() << std::endl;
    }

    std::cout << "Young:" << std::endl;
    YoungPersonView young = org.young();
    for (YoungPersonView::Iterator it = young.begin(); it != young.end(); ++it) {
        std::cout << it->name() << std::endl;
    }

    return 0;
}
#endif