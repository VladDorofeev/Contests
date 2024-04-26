#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <map>
#include <stack>

namespace solution {
typedef std::set<char> State;
typedef std::map<State, std::map<char, State>> Table;

typedef std::pair<char, std::string> Rule;
typedef std::set<Rule> Grammar;
typedef Grammar (GrammarFunc)(); 

constexpr char BEGIN_STATE = '@';
constexpr char ERROR_STATE = '#';

Table get_table(GrammarFunc get_grammar) {
    Grammar g = get_grammar();
    State start = {BEGIN_STATE};
    Table t;
    std::for_each(g.begin(), g.end(), 
    [&](const Rule &r)
    {
        char from;
        char to;
        char name;

        size_t len_right = r.second.length();
        if (len_right == 2) {
            // T->Ka
            from = r.second[0];
            name = r.second[1];
            to = r.first;

            bool found = false;
            for (Table::iterator p_table = t.begin(); p_table != t.end(); p_table++) {
                if (std::find(p_table->first.begin(), p_table->first.end(), from) != p_table->first.end()) {
                    found = true;
                    p_table->second[name].insert(to);
                }
            }

            if (!found) {
                t[State({from})][name] = State({to});
            }

        }
        if (len_right == 1) {
            // T->a
            //from = start; but start type of State
            name = r.second[0];
            to = r.first;

            t[start][name].insert(to);
        }
    });

    return t;

}

std::map<char, State> operator+(const std::map<char, State>& a, const std::map<char, State>& b) {
    std::map<char, State> c = a;

    for (const auto& pair : b) {
        char key = pair.first;
        const auto& b_set = pair.second;
        auto& c_set = c[key];

        c_set.insert(b_set.begin(), b_set.end());
    }
    
    return c;
}

void determine(Table &t) {
    Table det_t;
    State start = {BEGIN_STATE};
    det_t[start] = t[start];
    
    std::stack<State> queue;

    std::for_each(det_t[start].begin(), det_t[start].end(),
    [&](const auto &edge)
    {
        queue.push(edge.second);
    });

    std::map<char, State> edges_for_state;
    while (!queue.empty()) {
        edges_for_state.clear();
        State cur_state = queue.top();
        queue.pop();

        std::for_each(cur_state.begin(), cur_state.end(), [&](char nonterm)
        {
            edges_for_state = edges_for_state  + t[State({nonterm})];
        });

        det_t[cur_state] = edges_for_state;

        std::for_each(det_t[cur_state].begin(), det_t[cur_state].end(),
        [&](const auto &edge)
        {
            if (det_t.find(edge.second) == det_t.end()) {
                queue.push(edge.second);
            }
        });
    }
    t.clear();
    t = det_t;
} 

bool belong(Table t, std::string &s) {
    State cur_state = {BEGIN_STATE};
    State err_state = {ERROR_STATE};
    for (char sym: s) {
        if ((sym == '_') && (t[cur_state].find(sym) == t[cur_state].end())) {
            break;
        }
        if ((sym == '_') && (t[cur_state].find(sym) != t[cur_state].end())) {
            return true;
        }

        if (t[cur_state].find(sym) != t[cur_state].end()) {
            cur_state = t[cur_state][sym];
        } else {
            cur_state = err_state;
            break;
        }
    }
    if (cur_state == err_state) {
        return false;
    }
    if (cur_state.find('S') == cur_state.end()) {
        return false;
    }
    return true;
}

void print(Table &t) {
    for (Table::iterator p_table = t.begin(); p_table != t.end(); p_table++) {
        std::cout << "(";
        std::for_each(p_table->first.begin(), p_table->first.end(), [](char c){std::cout << c;});
        std::cout << ")" << std::endl; 
        for (auto p_edge = p_table->second.begin(); p_edge != p_table->second.end(); p_edge++) {
            
            std::cout << "       " << p_edge->first;
            
            std::cout << "(";
            for (auto iter = p_edge->second.begin(); iter != p_edge->second.end(); iter++) {
                std::cout << *iter;
            }
            std::cout << ")";
            std::cout << std::endl;

        }
        std::cout << std::endl;
    }
}
}

std::set<std::pair<char, std::string>> g();

#ifdef _main
std::set<std::pair<char, std::string>> g() {
    return {
        {'S', "Ka"},
        {'K', "b"},
    };
}
#endif

int main() {
    solution::Table table = solution::get_table(g);
    solution::determine(table);

    std::string s;
    while (std::getline(std::cin, s)) {
        s += '_';
        if (solution::belong(table, s)) {
            std::cout << "YES" << std::endl;
        } else {
            std::cout << "NO" << std::endl;
        }
    }
}
