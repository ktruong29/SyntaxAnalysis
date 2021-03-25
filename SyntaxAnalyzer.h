#pragma once

#include <iostream>
#include <ctype.h>
#include <string>
#include <fstream>
#include <list>
using namespace std;

struct Tokens
{
  string lex;
  string tok;
};

class SyntaxAnalyzer
{
  public:
    SyntaxAnalyzer();
    ~SyntaxAnalyzer();

    void Push(string tok, string lex);
    void Pop();
    bool IsEmpty();
    void PrintAll();
    void GrammarCheck();

  private:
    list<Tokens> tokenLists;
    bool D();     //Declarative
    bool T(string type);  //Type
    Tokens PopAndGetNextToken();
};
