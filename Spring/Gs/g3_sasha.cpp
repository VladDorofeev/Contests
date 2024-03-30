#include <iostream>

class Action {
public:
    virtual void run() = 0;
    virtual ~Action() = default;
};


class TokenizedAction: public Action {
public:
    TokenizedAction(TokenPool&, Action&);
    ~TokenizedAction();

    void run();
private:
    TokenPool& tokens;
    Action & action;
    std::string token;
};


TokenizedAction::TokenizedAction(TokenPool & _tokens, Action & _action)
    : tokens(_tokens)
    , action(_action)
{
    token.resize(tokens.token_length());
    tokens.get_token(token.data());
}


TokenizedAction::~TokenizedAction()
{
    tokens.return_token(token.c_str());
}


void TokenizedAction::run() {
    action.run();
}