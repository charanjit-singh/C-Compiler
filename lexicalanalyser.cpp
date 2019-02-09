#include "lexicalanalyser.h"
#include<iostream>
#include<regex>
#include<vector>
#include<stack>
#include<string>
#include <algorithm>


using namespace std;


LexicalAnalyser::LexicalAnalyser(ifstream &program)
{
    this->programFileStream = &program;

    // preparing prims
    for(auto i:this->KEYWORDS)
    {
        if(i!="")
        {
            this->prims.push_back(i);
        }
    }
    for(auto i:this->PUNCTUATORS)
    {

        if(i!="")
        {
            this->prims.push_back(i);
        }

    }
    // finish preparing prims
}

ifstream* LexicalAnalyser::get_program()
{
    return this->programFileStream;
}

void LexicalAnalyser::generate_program_parts()
{
    string tempPart="";
    string::iterator it;

    bool isStringStarted= false;
    bool isNextEscape = false;
    string program;
    string programLine="";

    while(getline(*this->programFileStream,programLine))
    {
        program+="\n"+programLine;
    }
    cout<<"Program:\n";
    cout<<program;


    for(it=program.begin(); it!=program.end();it++)
    {
        /*
         * While recognising a string, the characters '"' will be appended to the actual content, in the begining and the end.
         *
        */
        char character = *it;

        if(isStringStarted)
        {

            tempPart.push_back(character);  // no matter what, everything will be pushed into string

            if(isNextEscape)
            {
                isNextEscape=false;
                continue;
            }

            if(character=='"')
            {
                isStringStarted=false;
                Token temptoken(STRINGLITERAL, tempPart, 0);
                this->TokenStream.push_back(temptoken);

                tempPart.resize(0);
                continue;
            }

            if(character=='\\')
            {
                isNextEscape=true;
                continue;
            }

            continue;

        }
        else
        {
            if(!(character==' '|| character=='\n'|| character=='\t'))
            {
                if(character=='\'')
                {
                    tempPart.push_back(*(it));
                    if(*(it+2)=='\'')
                    {
                        // valid character
                        it++;
                        tempPart.push_back(*(it));
                        it++;
                        tempPart.push_back(*(it));
                        // push_back
                        // genrate new program part with it, it+1, it+2
                        continue;
                        // if *(it+3) == '\' then generate program part with it, it+1, it+2, it+3
                    }
                    else
                    {
                        // invalid character
                        // raise exception
                    }
                }

                string string_character="";
                string_character.push_back(character);
                if(this->is_punctuator(string_character))
                {
                    // flush whatever is until now
                    if(tempPart.size()>0)
                    {

                        appendTokenStream(tempPart);
                        tempPart.resize(0);
                    }
                    // get next character and then check if it is in punctuator,
                    // then push that to tempPart seperately
                    tempPart.push_back(character);
                     string_character.push_back(*(it+1));

                     if(this->is_punctuator(string_character))
                     {
                        tempPart.push_back(*(it+1));
                        Token temptoken(PUNCTUATOR, tempPart, 0 );
                        this->TokenStream.push_back(temptoken);
                        it++;
                        tempPart.resize(0);
                        continue;
                     }
                     else
                     {
                         Token temptoken(PUNCTUATOR, tempPart, 0 );
                         this->TokenStream.push_back(temptoken);
                         tempPart.resize(0);
                         continue;
                     }

                }
                if(character=='"')
                {
                    isStringStarted=true;
                    tempPart.push_back(character);
                    continue;
                }
                tempPart.push_back(character);
            }
            else if(character==' '|| character=='\n'|| character=='\t')
            {
                if(tempPart.size()>0)
                {
                    appendTokenStream(tempPart);
                }
                tempPart.resize(0);
                continue;
            }
            continue;
        }

    }
    // if stringStarted then raise exception
    cout<< "IsString: "<<isStringStarted<<" isNextEscape: "<<isNextEscape<<" tempPart"<<tempPart<<'\n';
    cout<<"------------------------------------------------------------------------------------------\n";

}

vector<Token> LexicalAnalyser::tokenize()
{
//    generate program parts and tokenize

    this->generate_program_parts();\
    // tokenize
    for(auto i: this->TokenStream)
    {

    }
    return this->TokenStream;
}

void LexicalAnalyser::appendTokenStream(string a)
{
    TokenTypes tokenType = UNKNOWN;
    map<string, string> extras;
    if(this->is_identifier(a))
    {
        tokenType= IDENTIFIER;
    }
    if(this->is_keyword(a))
    {
        tokenType = KEYWORD;
    }
    if(this->is_constant(a, extras ))
    {
        tokenType = CONSTANT;
    }
    Token token(tokenType,a,0);
    token.extras = extras;
    this->TokenStream.push_back(token);
}

bool LexicalAnalyser::is_punctuator(string a)
{
    // check in this->PUNCTUATORS

    for(auto i: this->PUNCTUATORS)
    {
        if(i!="" and i==a)
        {
            return true;
        }
    }
    return false;
}

bool LexicalAnalyser::is_keyword(string a)
{
    // check in this->KEYWORDS

    for(auto i: this->KEYWORDS)
    {
        if(i!="" and i==a)
        {
            return true;
        }
    }
    return false;
}

bool LexicalAnalyser::is_identifier(string a)
{
    return regex_match(a, regex("[_a-zA-Z][_a-zA-Z0-9]*"));
}

bool LexicalAnalyser::is_constant(string a, map<string,string>& extras)
{

    LEXICAL_ERR err("");
    extras["type"] = "UNKNOWN";

    if(regex_match(a, regex("0?[x|X]?\[0-9A-Fa-f]+(u|U|l|L|LL|ll|ul|uL|ull|uLL|Ul|UL|Ull|ULL|lu|Lu|lU|LU|llu|llU|LLU|LLu)?")))
    {

        extras["type"] = "INT";
        extras["unsigned"] = "NO";
        extras["base"] = "UNKNOWN";
        extras["length"] = "STANDARD"; // STANDARD, SHORT, LONG, LONG_LONG

        smatch suffix_match;
        regex suffix_expr("(u|U|l|L|LL|ll|ul|uL|ull|uLL|Ul|UL|Ull|ULL|lu|Lu|lU|LU|llu|llU|LLU|LLu)$");
        regex_search(a, suffix_match, suffix_expr); // check if it has suffix

        if(suffix_match.size())
        {
            int position = suffix_match.position(0); // position of suffix match
            string suffix =  a.substr(position);
            string remaining = a.substr(0, position); // strip suffix  here
            a = remaining;

            transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower); // to lower all string
            if(suffix.find("u")!=string::npos)
            {
                // 'a' is unsigned
                extras["unsigned"] = "YES";
            }
            if(suffix.find("l")!=string::npos)
            {
                if(suffix.find("ll")!=string::npos)
                {
                   // 'a' is long long
                    extras["length"] = "LONG_LONG";
                }
                else
                {
                    // 'a' is long
                    extras["length"] = "LONG";
                }
            }
        }

        // checking base of the integer constant
        if(regex_match(a, regex("((^[1-9]+\\d*$)|(^[0]$))")))
        {
            extras["base"] = "10";
        }

        if(regex_match(a, regex("^(0[x|X][0-9A-Fa-f]+)$")))
        {
            extras["base"] = "16";

        }

        if(regex_match(a, regex("^(0[0-7]+)$")))
        {
            extras["base"] = "8";
        }

        if(extras["base"]=="UNKNOWN")
        {
            err.setMsg("Could not identify base of integer.");
            //throw err;
        }

        return true;
    }
    return false;
}

void LexicalAnalyser::printTokenStream()
{
    string TokenTypeStrings[] = {
                               "KEYWORD",
                               "IDENTIFIER",
                               "CONSTANT",
                               "STRINGLITERAL",
                               "PUNCTUATOR",
                               "UNKNOWN"
                               };
    int counter = 0;
    for(auto i:this->TokenStream)
    {
        cout<<"Token :"<<counter<<'\n';

        cout<<"\t"<<"TYPE: "<<TokenTypeStrings[i.type]<<'\n';
        if(i.type==CONSTANT)
        {
            cout<<"\t TYPE: "<<i.extras["type"]<<"\n";
            if(i.extras["type"]=="INT")
            {
                cout<<"\t UNSIGNED: "<<i.extras["unsigned"]<<'\n';
                cout<<"\t LENGTH: "<<i.extras["length"]<<'\n';
                cout<<"\t BASE:"<<i.extras["base"]<<"\n";
            }
        }
        cout<<'\t'<<"TEXT: "<<i.text<<'\n';
        cout<<"------------------------------\n";

        counter++;
    }

}

