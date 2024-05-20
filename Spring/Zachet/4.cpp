#include <iostream>
#include <exception>



template <class T>
class MultiSet
{
public:
    MultiSet();
    MultiSet(const MultiSet&, int new_cap = -1);
    MultiSet& operator=(MultiSet);
    ~MultiSet();

    void clear();
    int size() const;

    T& operator[](int);
    const T& operator[](int) const;

    void add(const T&);
    void del(int, T &);

    int capacity() const { return cap; } // Not necessary
private:
    void swap(MultiSet&);

    T *arr;
    int sz;
    int cap;
};

template <class T>
MultiSet<T>::MultiSet() :
    arr(nullptr),
    sz(0),
    cap(0)
{
}

template <class T>
void MultiSet<T>::swap(MultiSet<T> &other) {
    T *temp = arr;
    int temp_sz = sz;
    int temp_cap = cap;

    arr = other.arr;
    sz = other.sz;
    cap = other.cap;

    other.arr = temp;
    other.sz = temp_sz;
    other.cap = temp_cap;
}


template <class T>
MultiSet<T>::MultiSet(const MultiSet<T>& other, int _new_cap) :
    arr(nullptr),
    sz(0),
    cap(0)
{ 
    int new_cap = ((_new_cap >= other.cap) && (_new_cap > 0)) ? _new_cap : other.cap;
    
    T *temp = new T[new_cap];
    
    for (int i = 0; i < other.sz; i++) {
        try
        {
            temp[i] = other.arr[i];
        }
        catch(const std::exception &err)
        {
            delete[] temp;
            throw;
        }
        
    }

    arr = temp;
    sz = other.sz;
    cap = new_cap;
}

template <class T>
MultiSet<T>& MultiSet<T>::operator=(MultiSet<T> other) {
    if (this == &other) {
        return *this;
    }
    swap(other);
    return *this;
}

template <class T>
MultiSet<T>::~MultiSet() {
    delete[] arr;
    sz = 0;
    cap = 0;
}

template <class T>
void MultiSet<T>::clear() { sz = 0; }

template <class T>
int MultiSet<T>::size() const { return sz; }

template <class T>
T& MultiSet<T>::operator[](int index){
    if (index >= sz) {
        throw std::out_of_range("Index >= sz");
    }
    return arr[index];
}

template <class T>
const T& MultiSet<T>::operator[](int index) const {
    if (index >= sz) {
        throw std::out_of_range("Index >= sz");
    }
    return arr[index];
}

template <class T>
void MultiSet<T>::add(const T &elem) {
    if (cap == sz) {
        int new_cap = (cap == 0) ? 2 : (cap * 2);

        MultiSet<T> temp(*this, new_cap);
        swap(temp);
    }

    try
    {
        arr[sz] = elem;
        sz++;
    }
    catch(const std::exception &err)
    {
        throw;
    }
}

template<class T>
void MultiSet<T>::del(int index, T &elem) {
    T saved_elem = (*this)[index];
    MultiSet<T> temp(*this);
    
    try
    {
        (*this)[index] = arr[sz-1];
        elem = saved_elem;
        sz--;
    }
    catch(const std::exception& e)
    {
        swap(temp);
        throw;
    }
    
}

template<class T>
void print(const MultiSet<T> &set) {
    std::cout << "==============================" << std::endl;
    if (set.size() == 0) {
        std::cout << "clear set " << std::endl;
        return;
    }

    std::cout << "sz = " << set.size() << " cap = " << set.capacity() << std::endl;
    for (int i = 0; i < set.size(); i++) {
        std::cout << set[i] << ' ';
    }    
    std::cout << std::endl;
    std::cout << "==============================" << std::endl;
}


class Component
{
public:
    Component() = default;
    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;
    virtual ~Component() = default;

    int count() const;
    Component& subcomponent(int);
    const Component& subcomponent(int) const;
    void add(Component &);

    friend std::ostream& operator<<(std::ostream &, const Component &);
protected:
    MultiSet<Component *> set;
private:
    virtual const std::string& name() const = 0;
};

int Component::count() const { return set.size(); }

Component& Component::subcomponent(int index) {
    return *set[index];
}

const Component& Component::subcomponent(int index) const {
    return *set[index];
}

void Component::add(Component &elem) {
    set.add(&elem);
}

std::ostream& operator<<(std::ostream &in, const Component &component) {
    in << component.name();
    in << '(';
    for (int i = 0; i < component.count(); i++) {
        in << component.subcomponent(i);
        if (i != component.count() - 1) {
            in << ", ";
        }
    }
    in << ')';
    return in;
}

class Box : public Component
{
public:
    Box(const std::string &);
    Box(const Box &);
    Box& operator= (const Box &);
    ~Box() = default;
private:
    virtual const std::string& name() const;
    std::string s;
};

Box::Box(const std::string &_s) : 
    Component(),
    s(_s)
{
}

Box::Box(const Box &other) :
    Component(),
    s(other.s)
{
}

Box& Box::operator= (const Box &other) {
    if (this == &other) {
        return *this;
    }

    std::string temp(other.s);
    set.clear();
    s.swap(temp);

    return *this;
}

const std::string& Box::name() const {
    static std::string _name("qwerty");
    // return _name; 
    return s;
}


struct Pair
{
    std::string name;
    Component *component;
};

/*
    //infty space syms
    Tree     = Node
    Node     = List | NonList
    List     = Name()
    NonList  = Name (SubTree)
    Subtree  = Tree | SubTree, Tree
    Name     = chars


    Tree    -> Node ( SubTree )
    SubTree -> eps | Tree <push add> {,Tree <push add> }
    Node    -> char + some chars and digits <push name>

    a(b(c()), d)
    a b c add add d add
*/

class Parser
{
public:
    Parser() = default;

    void parse(const std::string &s, const MultiSet<Pair> &_components);
    
    Component* poliz_interpret();
private:
    int c;
    
    void gc();
    std::string s;
    std::size_t pos;

    void Tree();
    void SubTree();
    void Node();

    MultiSet<Pair> components;
    MultiSet<Component*> poliz;
    
    MultiSet<std::string> names;

};

void Parser::parse(const std::string &_s, const MultiSet<Pair> &_components) {
    components = _components;
    poliz.clear();    
    names.clear();
    
    s = _s;
    pos = 0;

    gc();
    Tree();

    if (c != EOF) {
        throw std::logic_error("Not EOF");
    }



}
    
void Parser::gc() {
    if (pos == s.length()) {
        c = EOF;
        return;
    }
    c = s.at(pos++);
    while (std::isspace(c)) {
        if (pos == s.length()) {
            c = EOF;
            return;
        }
        c = s.at(pos++);
    }
}

void Parser::Tree() {
    Node();
    if (c == '(') {
        gc();
        SubTree();
        if (c != ')') {
            throw std::logic_error("Not enough )");
        }
        gc();
    } else {
        throw std::logic_error("Not enogh (");
    }
}

void Parser::SubTree() {
    if (std::isalpha(c)) {
        Tree();
        poliz.add(nullptr);
        while (c == ',') {
            gc();
            Tree();
            poliz.add(nullptr);
        }
    }
}

void Parser::Node() {
    std::string name;
    if (std::isalpha(c)) {

        // < //
        name.append(1, static_cast<char>(c));
        // > //
        
        gc();
        while(std::isalnum(c)) {
            
            // < //
            name.append(1, static_cast<char>(c));
            // > //
            
            gc();
        }

        // < //
        Component *ptr = nullptr;
        for (int i = 0; i < components.size(); i++) {
            if (components[i].name == name) {
                ptr = components[i].component;
            }
        }

        if (ptr == nullptr) {
            throw std::logic_error("Undefind name of Node");
        }

        for (int i = 0; i < names.size(); i++) {
            if (names[i] == name) {
                throw std::logic_error("Double use variable");
            }
        }
        names.add(name);
        poliz.add(ptr);
        // > //

    } else {
        throw std::logic_error("Bad Name");
    }
}

Component* Parser::poliz_interpret() {

    MultiSet<Component*> stack;
    for (int i = 0; i < poliz.size(); i++) {
        if (poliz[i] != nullptr) {
            stack.add(poliz[i]);
        } else {
            Component *op2;
            stack.del(stack.size() - 1, op2);
            Component *op1;
            stack.del(stack.size() - 1, op1);
            
            op1->add(*op2);
            stack.add(op1);
        }
    }

    Component *root;
    stack.del(0, root);
    stack.clear();
    return root;
}


Component *make_from_text(const std::string &text, const MultiSet<Pair> &components) {
    Parser parser;
    parser.parse(text, components);
    return parser.poliz_interpret();
}


int main() {
    MultiSet<Pair> components;
    Box A1("a1"), B1("b1"), C1("c1"),
    D1("d1"), G1("g1"), H1("h1"), L1("l1"), M1("m1");
    components.add({"a1", &A1});
    components.add({"b1", &B1});
    components.add({"c1", &C1});
    components.add({"d1", &D1});
    components.add({"g1", &G1});
    components.add({"h1", &H1});
    components.add({"l1", &L1});
    components.add({"m1", &M1});
    Component * node = nullptr;
    try {
        node = make_from_text("c1(a1(b1()),d1(h1(l1(),m1())))",
        components);
    }
    catch (std::exception& err) {
        std::cout << err.what() << std::endl;
        return 0;
    }
    std::cout << *node << std::endl;
    return 0;
}