#include <iostream>
#include<string>
#include<fstream>
#include<regex>
#include"lexicalanalyser.h"
using namespace std;

int main()
{
    ifstream programFile;
    programFile.open("../mCompiler/input_file");
    if(!programFile.is_open())
    {
        cout<<"Could not open program file.";
        exit(-1);
    }
//    string SampleProgram="int main(){ int i = 0 ;\t sting a=\"asas \t\t  adsa\"  return 0 ; } ";

//    string a[]={"1","123","0x123","0X23","0xasd","12u","1221l","12323U","123132L","0x1233l","0X123U","0X123LU"};
//    for(auto i:a)
//    {
//            smatch m;
//            regex e("(u|U|l|L|LL|ll|ul|uL|ull|uLL|Ul|UL|Ull|ULL|lu|Lu|lU|LU|llu|llU|LLU|LLu)$");
//            regex_search( i, m, e );
//            cout<<i<<" Matches: "<<m.size()<<' ';
//            for (unsigned j=0; j<; ++j) {
//              cout << i << "  match " << j << " (" << m[j] << ") ";
//              cout << "at position " << m.position(j) << endl;
//            }

//    }

    LexicalAnalyser lexicalAnalyser(programFile);
    lexicalAnalyser.tokenize();
    lexicalAnalyser.printTokenStream();
    return 0;
}
