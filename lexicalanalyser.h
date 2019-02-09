#ifndef LEXICALANALYSER_H
#define LEXICALANALYSER_H

#include<string>
#include<fstream>
#include<vector>
#include<map>
#include<exception>

#define TOTAL_KEYWORDS_LENGTH 40
#define TOTAL_PUNCTUATORS_LENGTH 50

using namespace std;
enum TokenTypes {
    KEYWORD,
    IDENTIFIER,
    CONSTANT,
    STRINGLITERAL,
    PUNCTUATOR,
    UNKNOWN
};

class Token{
public:
    string text;
    int priority;
    TokenTypes type;
    map<string, string> extras;

    Token(TokenTypes type=UNKNOWN, string text="", int priority=0 ); // constructor


};

class LexicalAnalyser
{
private:
    ifstream *programFileStream;
    vector<string> ProgramParts;
    void generate_program_parts();
    vector<string> prims; // list of all primitives
    void appendTokenStream(string);


public:
    vector<Token> TokenStream;

    LexicalAnalyser(ifstream& program); // constructor

    ifstream* get_program(); // returns program in use
    vector<Token> tokenize(); // Returns token Stream after tokenizing the program

    void printTokenStream();

    bool is_keyword(string);
    bool is_constant(string, map<string, string>&);
    bool is_punctuator(string);
    bool is_identifier(string);


    const string KEYWORDS[TOTAL_KEYWORDS_LENGTH]={
        "auto",
        "break",
        "case",
        "char",
        "const",
        "continue",
        "default",
        "do",
        "double",
        "else",
        "enum",
        "extern",
        "float",
        "for",
        "goto",
        "if",
        "inline",
        "int",
        "long",
        "register",
        "restrict",
        "return",
        "short",
        "signed",
        "sizeof",
        "static",
        "struct",
        "switch",
        "typedef",
        "union",
        "unsigned",
        "void",
        "this",
        "volatile",
        "while",
        "_Bool",
        "_Complex",
        "_Imaginary"
    };

    const string PUNCTUATORS[TOTAL_PUNCTUATORS_LENGTH]={
        "{",
        "}",
        "[",
        "]",
        ",",
        ".",
        ";",
        "(",
        ")",
        "<",
        ">",
        "<=",
        ">=",
        "++",
        "--",
        "+",
        "-",
        "*",
        "/",
        "+=",
        "-=",
        "*=",
        "/=",
        "%",
        "&",
        "!",
        "!=",
        "|",
        "|=",
        "||",
        "&&",
        "?",
        "=",
        "->",
        "==",
        "<%",
        "%>"
    };

};

class LEXICAL_ERR: public exception {
public:
    string msg;
    LEXICAL_ERR(string desc) :msg(desc) {}
    ~LEXICAL_ERR() throw() {}
    void setMsg(string msg){this->msg =msg;}
    virtual const char * what () const throw () { return msg.c_str(); }
};

#endif // LEXICALANALYSER_H
