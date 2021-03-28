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
    void PrintAll(ofstream &fout);
    void GrammarCheck(ofstream &fout);

  private:
    list<Tokens> tokenLists;
    Tokens PopAndGetNextToken(ofstream &fout);
    /***************************************************************************
     * The following methods D, DPrime, and Type are representing the productions:
     *  <D>       -> <Type> id <DPrime> ; | epsilon
     *  <DPrime>  -> , id <DPrime> | epsilon
     **************************************************************************/
    bool D(ofstream &fout);     //Declarative
    bool DPrime(string curTok, string curLex, ofstream &fout);
    bool Type(string type);  //Type
    /***************************************************************************
     * The following methods E, EPrime, T, TPrime, F, and id are representing
     * the productions of:
     *  <E>       -> <T> <EPrime>
     *  <EPrime>  -> + <T> <EPrime> | - <T> <EPrime> |epsilon
     *  <T>       -> <F> <TPrime>
     *  <TPrime>  -> * <F> <TPrime> | / <F> <TPrime> | epsilon
     *  <F>       -> ( <E> ) | i | num
     *  i         -> id
     **************************************************************************/

};
