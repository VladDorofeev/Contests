#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <map>

namespace solution
{

typedef std::pair<char, std::string> Rule_type;
typedef std::set<Rule_type> Grammar_type;
typedef std::set<Rule_type> (Grammar_function)();
typedef int State_Type;

class DFA
{
public:
    DFA(Grammar_function g);
    DFA(const DFA &) = delete;
    DFA& operator=(const DFA &) = delete;
    ~DFA() = default;

    bool is_belong(const std::string &);


    void print_grammar() {std::for_each(grammar.begin(),grammar.end(), [](const Rule_type &r){std::cout << r.first << " -> " << r.second << std::endl;});}
    void print_all();
private:
    Grammar_type grammar;

    struct Edge
    {
        Edge(int, char, int);
        
        bool operator<(const Edge &) const;
        
        int from;
        char name;
        int to;
    };

    std::set<State_Type> states;
    std::set<char> alphabet;
    std::set<Edge> edges;
    std::set<State_Type> begin_states;
    std::set<State_Type> end_states;

    static constexpr char BEGIN_STATE_CHAR = '@';
    static constexpr State_Type BEGIN_STATE = 0;
    static constexpr State_Type ERROR_STATE = -1;

    int cur_begin_state = BEGIN_STATE;

    void determinate();
};

DFA::Edge::Edge(int _from, char _name, int _to) :
    from(_from),
    name(_name),
    to(_to)
{
}

bool DFA::Edge::operator<(const Edge &other) const {
    if (from < other.from) return true;
    if (from > other.from) return false;

    if (name < other.name) return true;
    if (name > other.name) return false;

    return to < other.to;
}



DFA::DFA(Grammar_function g) :
    grammar(g())
{
    // A->Ba ~ B -a- A
    // A->a
    // A->
    states.insert(BEGIN_STATE);
    begin_states.insert(BEGIN_STATE);

    int new_state = BEGIN_STATE;
    std::map<char, int> naming_table;
    naming_table.insert(std::make_pair(BEGIN_STATE_CHAR, new_state++));

    std::for_each(grammar.begin(), grammar.end(),
    [&](const Rule_type &r)
    {
        if (naming_table.find(r.first) == naming_table.end()) {
            naming_table.insert(std::make_pair(r.first, new_state++));
        }

        states.insert(naming_table[r.first]);

        std::size_t len = r.second.length();
        
        if (len == 2) {
            // A->Ba
            alphabet.insert(r.second[1]);

            if (naming_table.find(r.second[0]) == naming_table.end()) {
                naming_table.insert(std::make_pair(r.second[0], new_state++));
            }

            states.insert(naming_table[r.second[0]]);

            edges.insert(Edge(naming_table[r.second[0]], r.second[1], naming_table[r.first]));

        } else if (len == 1){
            // A->a
            alphabet.insert(r.second[0]);
            edges.insert(Edge(this->BEGIN_STATE, r.second[0], naming_table[r.first]));
        } else {
            // A->epsilon 
            // TODO: Can it be or not??? 
            begin_states.insert(r.first);
        }

    });
    end_states.insert(naming_table['S']);
    std::for_each(naming_table.begin(), naming_table.end(), [&](auto p){std::cout << p.first << " ~ " << p.second << std::endl;});

    determinate();
}

void DFA::determinate() {

}


bool DFA::is_belong(const std::string &chain) {
    State_Type cur_state = cur_begin_state;
    for (std::size_t i = 0; i < chain.length(); i++) {
        std::cout << "PARSE " << i << " = " << chain[i] << std::endl;
        if (std::find(alphabet.begin(), alphabet.end(), chain[i]) != alphabet.end()) {
            bool moved = false;

            for (auto it = edges.begin(); it != edges.end(); it++) {
                const Edge &edge = *it;
                std::cout << "parse edge " << edge.from << ' ' << edge.name << ' ' << edge.to << std::endl;
                if ((edge.from == cur_state) && (edge.name == chain[i])) {
                    cur_state = edge.to;
                    std::cout << "moved!!!!!!!!!!!!!\n";
                    moved = true;
                    break;
                }
            }

            if (!moved) {
                // Can`t moved
                std::cout << "CAN`T MOVED\n";
                cur_state = ERROR_STATE;
            }
        } else {
            // Symbol not in alphabet
            std::cout << "SYM ISN`T IN ALPH\n";
            cur_state = ERROR_STATE;
        }

        if (cur_state == ERROR_STATE) {
            break;
        }

        std::cout << "\n";
    }

    if (cur_state == ERROR_STATE) {
        return false;
    } 
    if (std::find(end_states.begin(), end_states.end(), cur_state) == end_states.end()) {
        return false;
    }
    
    return true;
}

}


#ifdef _main
std::set<std::pair<char, std::string>> g() {
    return {
        std::make_pair('S', std::string("Aa")), 
        std::make_pair('S', std::string("a")), 
        std::make_pair('A', std::string("a")), 
    };
}
#endif

int main() {
    solution::DFA dfa(g);
    dfa.print_all();

    std::string s;
    while (std::getline(std::cin, s)) {
        if (dfa.is_belong(s) == true) {
            std::cout << "YES" << std::endl;
        } else {
            std::cout << "NO" << std::endl;
        }
        std::cout << '\n';
    }
    return 0;
}



void solution::DFA::print_all() 
    {
        std::cout << "===================================" << std::endl;
        std::cout << "DFA:" << std::endl;
        std::cout << "STATES: sz = " << states.size() << std::endl;
        std::cout << "  ";
        std::for_each(states.begin(), states.end(), [](State_Type c){std::cout << c << " , ";});
        std::cout << std::endl;
        std::cout << std::endl;

        std::cout << "ALPHABET: sz = " << alphabet.size() << std::endl;
        std::cout << "  ";
        std::for_each(alphabet.begin(), alphabet.end(), [](char c){std::cout << c << " , ";});
        std::cout << std::endl;
        std::cout << std::endl;

        std::cout << "EDGES: sz = " << edges.size() << std::endl;
        std::cout << "  ";
        std::for_each(edges.begin(), edges.end(), [](const Edge &edge)
        {
            std::cout << edge.from << " -" << edge.name << "- " << edge.to << std::endl;
            std::cout << "  ";
        });
        std::cout << std::endl;

        std::cout << "BEGIN_STATES: sz = " << begin_states.size() << std::endl;
        std::cout << "  ";
        std::for_each(begin_states.begin(), begin_states.end(), [](State_Type c){std::cout << c << " , ";});
        std::cout << std::endl;
        std::cout << std::endl;

        std::cout << "END_STATES: sz = " << end_states.size() << std::endl;
        std::cout << "  ";
        std::for_each(end_states.begin(), end_states.end(), [](State_Type c){std::cout << c << " , ";});
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "===================================" << std::endl;
    }