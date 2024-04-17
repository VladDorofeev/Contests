#include <iostream>
#include <vector>
#include <deque>
#include <list>

template<typename Container>
typename Container::value_type last3(const Container &cont) {
    if (cont.size() < 3) {
        throw std::invalid_argument("Container hasn`t 3 nums");
    }
    
    typename Container::const_reverse_iterator iter = cont.rbegin();

    typename Container::value_type sum = *iter++;
    sum = sum + *iter++;
    sum = sum + *iter++;

    return sum;
}



#ifdef _main
int main() {
    std::vector<std::string> vec;
    std::deque<std::string> deq;
    std::list<std::string> l;

    vec.push_back("111");
    vec.push_back("222");
    vec.push_back("333");
    
    deq.push_back("111");
    deq.push_back("222");
    deq.push_back("333");
    
    l.push_back("111");
    l.push_back("222");
    l.push_back("333");
    

    std::cout << last3(vec) << std::endl;    
    last3(deq);    
    last3(l);    
    
    return 0;
}
#endif