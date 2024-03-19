#include <iostream>
#include <cstring>
#include <cstdlib>

class Person {
public:
    Person(char const*, int, bool);
    Person(Person const&);
    ~Person();
    char const* name() const ;
    int age() const ;
    bool man() const ;
    bool young() const;
private:
    const char* _name;
    int _age;
    bool _is_man;
};

Person::Person(char const* p_name, int p_age, bool p_is_man) 
    : _name(p_name)
    , _age(p_age)
    , _is_man(p_is_man) 
{
}


Person::Person(Person const& other)
    : _name(other._name)
    , _age(other._age)
    , _is_man(other._is_man) 
{
}


Person::~Person() 
{
}


char const* Person::name() const {
    return _name;
}


int Person::age() const {
    return _age;
}


bool Person::man() const {
    return _is_man;
}


bool Person::young() const {
    return (_age >= 18 && _age <=35) ? true : false;
}


bool condition_man(Person* arr) {
    return arr->man();
}


bool condition_young(Person* arr) {
    return arr->young();
}

class SimplePersonView {
public:
    class Iterator {
    public:
        Iterator(Person*, int, bool (*func)(Person*));
        Iterator(Iterator const&);
        Person* operator->();
        const Person* operator->() const;
        friend bool operator!=(
            SimplePersonView::Iterator const&
            , SimplePersonView::Iterator const&
        );
        Iterator& operator++();
    private:
        Person* it_cur;
        Person* it_end;
        bool (*condition_func)(Person*);
    };
    Iterator begin();
    Iterator end();
protected:
    SimplePersonView(Person*, int, bool (*func)(Person*));
    SimplePersonView(SimplePersonView const&);
    Person* view_arr;
    int cnt;
    bool (*condition_func)(Person*);
};

SimplePersonView::SimplePersonView(Person* arr, int len, bool (*func)(Person*))
    : view_arr(arr)
    , cnt(len) 
    , condition_func(func)
{
}


SimplePersonView::SimplePersonView(SimplePersonView const& other) 
    : view_arr(other.view_arr)
    , cnt(other.cnt)
    , condition_func(other.condition_func)
{
}


SimplePersonView::Iterator::Iterator(Person* arr, int len, bool(*func)(Person*))
    : it_cur(arr)
    , it_end(arr + len)
    , condition_func(func)
{
}


SimplePersonView::Iterator::Iterator(SimplePersonView::Iterator const& other)
    : it_cur(other.it_cur)
    , it_end(other.it_end)
    , condition_func(other.condition_func)
{
}


Person* SimplePersonView::Iterator::operator->() {
    return it_cur;
}


const Person* SimplePersonView::Iterator::operator->() const {
    return it_cur;
}


bool operator!=(
    SimplePersonView::Iterator const& it1
    , SimplePersonView::Iterator const& it2
) 
{
    return it1.it_cur != it2.it_cur;
}


SimplePersonView::Iterator& SimplePersonView::Iterator::operator++() {
    while(it_cur != it_end) {
        ++it_cur;
        if ((it_cur != it_end) && condition_func(it_cur)) {
            break;
        }
    }
    return *this;
}


SimplePersonView::Iterator SimplePersonView::begin() {
    int new_cnt = 0;
    Person* end_view = view_arr + cnt;
    while(view_arr != end_view && !condition_func(view_arr)) {
        ++view_arr;
        ++new_cnt;
    }
    return Iterator(view_arr, cnt - new_cnt, condition_func);
}


SimplePersonView::Iterator SimplePersonView::end() {
    return Iterator(view_arr + cnt, 0, condition_func);
}


class MenPersonView : public SimplePersonView {
public:
    MenPersonView(Person*, int);
    MenPersonView(MenPersonView const&);
};

MenPersonView::MenPersonView(Person* arr, int len)
    : SimplePersonView(arr, len, &condition_man)
{   
}


MenPersonView::MenPersonView(MenPersonView const& other)
    : SimplePersonView(other)
{
}


class YoungPersonView : public SimplePersonView {
public:
    YoungPersonView(Person*, int);
    YoungPersonView(YoungPersonView const&);
};

YoungPersonView::YoungPersonView(Person* arr, int len)
    : SimplePersonView(arr, len, &condition_young)
{   
}


YoungPersonView::YoungPersonView(YoungPersonView const& other) 
    : SimplePersonView(other)
{   
}


class Persons {
public:
    Persons();
    Persons(Persons const&);
    ~Persons();
    Persons& operator=(Persons const&);
    void add(Person const&);
    void write_names() {
        for(int i = 0; i < cnt; ++i) {
            std::cout << per_arr[i].name() << std::endl;
        }
    }
    MenPersonView men();
    YoungPersonView young();
protected:
    int cnt;
    int cnt_man;
    int cnt_young;
    int capacity;
    Person* per_arr;
};

Persons::Persons()
    : cnt(0)
    , cnt_man(0)
    , cnt_young(0)
    , capacity(0) 
    ,per_arr(nullptr)
{
}


Persons::Persons(Persons const& other) 
    : cnt(other.cnt)
    , cnt_man(other.cnt_man)
    , cnt_young(other.cnt_young)
    , capacity(other.capacity) 
    ,per_arr(cnt ? static_cast<Person*>(operator new[] (capacity*sizeof(Person))) : nullptr)
{
    for(int i = 0; i < cnt; ++i) {
        new (per_arr + i) Person(other.per_arr[i]);
    }
}


Persons::~Persons() {
    for(int i = 0; i < cnt; ++i) {
            per_arr[i].~Person();
    }
    operator delete[] (per_arr);
}


Persons& Persons::operator=(Persons const& other) {
    if (per_arr != other.per_arr) {
        for(int i = 0; i < cnt; ++i) {
            per_arr[i].~Person();
        }
        operator delete[] (per_arr);
        per_arr = nullptr;
        cnt = other.cnt;
        capacity = other.capacity;
        per_arr = static_cast<Person*>(operator new[] (capacity*sizeof(Person)));
        for(int i = 0; i < cnt; ++i) {
            new (per_arr + i) Person(other.per_arr[i]);
        }
    }
    return *this;
}


void Persons::add(Person const& new_per) {
    Person* new_per_arr = nullptr;
    if (cnt == capacity) {
        capacity = capacity ? capacity*2 : 20;
        ++capacity;
        new_per_arr = static_cast<Person*>(operator new[] (capacity*sizeof(Person)));
        memcpy(static_cast<void*>(new_per_arr), static_cast<void*>(per_arr), sizeof(Person) * cnt);
        operator delete[] (per_arr);
        per_arr = new_per_arr;
    }
    if (new_per.man()) {
        ++cnt_man;
    }
    if (new_per.young()) {
        ++cnt_young;
    }
    new(per_arr + cnt) Person(new_per);
    ++cnt;
}


MenPersonView Persons::men() {
    return MenPersonView(per_arr, cnt);
}


YoungPersonView Persons::young() {
    return YoungPersonView(per_arr, cnt);
}


int compare_man(const void * arg1, const void * arg2) {
    if (static_cast<const Person*>(arg1)->man() && !static_cast<const Person*>(arg2)->man()) {
        return -1;
    } else if (!static_cast<const Person*>(arg1)->man() && static_cast<const Person*>(arg2)->man()) {
        return 1;
    }
    return 0;
}


int compare_young(const void * arg1, const void * arg2) {
    if (static_cast<const Person*>(arg1)->young() && !static_cast<const Person*>(arg2)->young()) {
        return -1;
    } else if (!static_cast<const Person*>(arg1)->young() && static_cast<const Person*>(arg2)->young()) {
        return 1;
    }
    return 0;
}

class FastPersons : public Persons {
    public:
        MenPersonView men();
        YoungPersonView young();
};

MenPersonView FastPersons::men() {
    if (per_arr) {
        qsort(per_arr, cnt, sizeof(Person), compare_man);
    }
    return MenPersonView(per_arr, cnt_man);
}


YoungPersonView FastPersons::young() {
    if (per_arr) {
        qsort(per_arr, cnt, sizeof(Person), compare_young);
    }
    return YoungPersonView(per_arr, cnt_young);
}


/*int
main() {
    sons org;
    org.add(Person("Ivanov", 20, true));
    org.add(Person("Ivanova", 30, false));
    org.add(Person("Petrov", 50, true));
    org.add(Person("Sidorov", 32, true));
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
        FastPersons org;
    org.add(Person("Ivanov", 20, true));
    org.add(Person("Ivanova", 30, false));
    org.add(Person("Petrov", 50, true));
    org.add(Person("Sidorov", 32, true));
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
}*/