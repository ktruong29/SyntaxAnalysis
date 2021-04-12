#pragma once

#include <iostream>
#include <ctype.h>
#include <string>
#include <fstream>
#include <stdlib.h>
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
    bool flag;
    bool whileLoop;
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
     bool E(ofstream &fout);
     bool EPrime(ofstream &fout);
     bool T(ofstream &fout);
     bool TPrime(ofstream &fout);
     bool F(ofstream &fout);
     /***************************************************************************
      * The following method A represents the productions:
      *  <A>       -> id = <E> ;
      **************************************************************************/
     bool A(ofstream &fout);
    /***************************************************************************
     * The following method S (Statement) represents the productions:
     *  <S>       -> <A> | <D> | <W> ;  W - while loop
     **************************************************************************/
     bool S(ofstream &fout);
     /***************************************************************************
      * The following methods W, C, and R represent the productions:
      *  <W>      -> 'while' <C> 'do' <S> 'whileend' ;
      *  <C>      -> <E> <CPrime>
      *  <CPrime> -> <R> <E> | epsilon
      *  <R>      -> < | >
      **************************************************************************/
      bool W(ofstream &fout);
      bool C(ofstream &fout);
      bool CPrime(ofstream &fout);
      bool R(ofstream &fout);
      /***************************************************************************
       * The following method I represent the productions:
       *  <I> -> 'if' <C> 'then' <S> 'else' <S> 'endif';
       *  <C>      -> <E> <CPrime>
       *  <CPrime> -> <R> <E> | epsilon
       *  <A>       -> id = <E> ;
       **************************************************************************/
       bool I(ofstream &fout);
};
