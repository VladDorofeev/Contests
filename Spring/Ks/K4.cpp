#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <map>
#include <stack>

typedef std::set<char> State;
typedef std::map<State, std::map<char, State>> Table;

typedef std::pair<char, std::string> Rule;
typedef std::set<Rule> Grammar;
typedef Grammar (GrammarFunc)(); 

constexpr char BEGIN_STATE = '@';

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
    std::cout << (t.find({'K', 'S'}) == t.end()) << std::endl;
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
        std::cout << "new stack element" << std::endl;
        
        edges_for_state.clear();
        State cur_state = queue.top();
        queue.pop();

        std::cout << "parse (";
        std::for_each(cur_state.begin(), cur_state.end(), [](char c) {std::cout << c;});
        std::cout << ")" << std::endl;

        std::for_each(cur_state.begin(), cur_state.end(), [&](char nonterm)
        {
            std::cout << "merge from " << nonterm << std::endl;
            std::cout << "t[State({nonterm})]  size = " <<  t[State({nonterm})].size() << std::endl;

            std::for_each(t[State({nonterm})].begin(), t[State({nonterm})].end(),
            [&](const auto &edge)
            {
                std::cout << "edge to merge equ " << std::endl;
                std::cout << edge.first << ' ' << (edge.first == 0) << std::endl;
                std::for_each(edge.second.begin(), edge.second.end(), [](char c) {std::cout << c;});
                std::cout << std::endl;
            });

            edges_for_state = edges_for_state  + t[State({nonterm})];
        });

        det_t[cur_state] = edges_for_state;

        std::for_each(det_t[cur_state].begin(), det_t[cur_state].end(),
        [&](const auto &edge)
        {
            std::cout << "next push" << std::endl;
            std::for_each(edge.second.begin(), edge.second.end(), [](char c) {std::cout << c;});
            std::cout << std::endl;
            if (det_t.find(edge.second) == det_t.end()) {
                std::cout << "pushed\n";
                queue.push(edge.second);
            }
        });
        std::cout << "----------" << std::endl;
    }
    t.clear();
    t = det_t;
} 

void print(Table &t) {
    std::cout << "========================================" << std::endl;
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
    std::cout << "========================================" << std::endl;
}

Grammar g() {
    return {
        {'S', "Sa"},
        {'S', "Ma"},
        {'S', "Ka"},
        {'S', "Mb"},
        {'S', "c"},
        {'M', "Ka"},
        {'M', "c"},
        {'K', "c"},
    };
}

int main() {
    Table table = get_table(g);
    print(table);
    determine(table);
    print(table);
    // std::string s;
    // while (std::getline(std::cin, s)) {
    //     if (belong(table, s)) {
    //         std::cout << "YES" << std::endl;
    //     } else {
    //         std::cout << "NO" << std::endl;
    //     }
    // }
}

/*
    Table t;

    for sym in s:
    t[cur_state] //set pair
    for_each t[cur_state]{
        pair p;
        if (p.first == cur_sym) {
            cur_state = p.second;
            break;
        }
    }

*/
