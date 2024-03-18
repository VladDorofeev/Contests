#include <iostream>

enum {WITHOUT_COND, MAN_COND, YOUNG_COND};

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

class SimplePersonView {
public:
    class Iterator {
    public:
        Iterator(Person*, int, int);
        Iterator(Iterator const&);
        Person* operator->();
        const Person* operator->() const;
        friend bool operator!=(
            SimplePersonView::Iterator const&
            , SimplePersonView::Iterator const&
        );
        Iterator& operator++();
    private:
        bool is_cond(int);
        Person* it_cur;
        Person* it_end;
        int condition;
    };
    Iterator end();
protected:
    SimplePersonView(Person*, int);
    SimplePersonView(SimplePersonView const&);
    Person* view_arr;
    int cnt;
};

SimplePersonView::SimplePersonView(Person* arr, int len)
    : view_arr(arr)
    , cnt(len) 
{
}


SimplePersonView::SimplePersonView(SimplePersonView const& other) 
    : view_arr(other.view_arr)
    , cnt(other.cnt)
{
}


SimplePersonView::Iterator::Iterator(Person* arr, int len, int cond = WITHOUT_COND)
    : it_cur(arr)
    , it_end(arr + len)
    , condition(cond)
{
}


SimplePersonView::Iterator::Iterator(SimplePersonView::Iterator const& other)
    : it_cur(other.it_cur)
    , it_end(other.it_end)
    , condition(other.condition)
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
        if (is_cond(condition) && (it_cur != it_end)) {
            break;
        }
    }
    return *this;
}


bool SimplePersonView::Iterator::is_cond(int cond) {
    switch (cond)
    {
    case WITHOUT_COND:
        return true;
    case MAN_COND:
        return it_cur->man() ? true : false;
    case YOUNG_COND:
        return it_cur->young() ? true : false;
    default:
        return false;
        break;
    }
}


SimplePersonView::Iterator SimplePersonView::end() {
    return Iterator(view_arr + cnt, 0);
}


class MenPersonView : public SimplePersonView {
public:
    MenPersonView(Person*, int);
    MenPersonView(MenPersonView const&);
    Iterator begin();
};

MenPersonView::MenPersonView(Person* arr, int len)
    : SimplePersonView(arr, len)
{   
}


MenPersonView::MenPersonView(MenPersonView const& other)
    : SimplePersonView(other)
{
}


MenPersonView::Iterator MenPersonView::begin() {
    Person* end_view_arr = view_arr + cnt;
    int new_cnt = 0;
    while((!view_arr->man()) && (view_arr != end_view_arr)) {
        ++new_cnt;
        ++view_arr;
    }
    return MenPersonView::Iterator(view_arr, cnt - new_cnt, MAN_COND);
}


class YoungPersonView : public SimplePersonView {
public:
    YoungPersonView(Person*, int);
    YoungPersonView(YoungPersonView const&);
    Iterator begin();
};

YoungPersonView::YoungPersonView(Person* arr, int len)
    : SimplePersonView(arr, len)
{   
}


YoungPersonView::YoungPersonView(YoungPersonView const& other) 
    : SimplePersonView(other)
{   
}


YoungPersonView::Iterator YoungPersonView::begin() {
    Person* end_view_arr = view_arr + cnt;
    int new_cnt = 0;
    while((!view_arr->young()) && (view_arr != end_view_arr)) {
        ++new_cnt;
        ++view_arr;
    }
    return YoungPersonView::Iterator(view_arr, cnt - new_cnt, YOUNG_COND);
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
private:
    int cnt;
    int capacity;
    Person* per_arr;
};

Persons::Persons()
    : cnt(0)
    , capacity(0) 
    ,per_arr(nullptr)
{
}


Persons::Persons(Persons const& other) 
    : cnt(other.cnt)
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
        memcpy(new_per_arr, per_arr, sizeof(Person) * cnt);
        operator delete[] (per_arr);
        per_arr = new_per_arr;
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


int
main() {
    Persons org;
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
}