#include <iostream>
#include <exception>

template<typename Cont>
typename Cont::value_type last3(Cont const& cont) {
    if (cont.size() < 3) {
        throw std::exception();
    }
    typename Cont::const_iterator it = cont.end();
    --it;
    --it;
    --it;
    return *it++ + *it++ + *it;
}