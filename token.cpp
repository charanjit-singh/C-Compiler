#include"lexicalanalyser.h"
#include "regex"



Token::Token(TokenTypes type, string text, int priority ){
    this->type=type;
    this->text=text;
    this->priority=priority;
}
