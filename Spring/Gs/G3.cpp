#include <iostream>

class Action
{
public:
    virtual void run() = 0;
    virtual ~Action() = default;
};


class TokenizedAction : public Action
{
public:
    TokenizedAction(TokenPool &_pool, Action &_action);
    ~TokenizedAction();

    void run();
private:
    TokenPool &pool;
    Action &action;
    std::string token;
};


TokenizedAction::TokenizedAction(TokenPool &_pool, Action &_action) :
    pool(_pool),
    action(_action)
{
    token.resize(pool.token_length());
    pool.get_token(token.data());
}


TokenizedAction::~TokenizedAction() {
    pool.return_token(token.c_str());
}


void TokenizedAction::run() {
    action.run();
}
