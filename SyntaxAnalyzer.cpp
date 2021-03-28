#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(){}

SyntaxAnalyzer::~SyntaxAnalyzer(){}

void SyntaxAnalyzer::Push(string tok, string lex)
{
  Tokens currToken;
  currToken.lex = lex;
  currToken.tok = tok;
  // fout << "Lex: " << lex << " Token: " << tok << endl;
  tokenLists.push_back(currToken);
}

void SyntaxAnalyzer::Pop()
{
  if(!IsEmpty())
  {
    tokenLists.pop_front();
  }
}

bool SyntaxAnalyzer::IsEmpty()
{
  return tokenLists.empty();
}

void SyntaxAnalyzer::PrintAll(ofstream &fout)
{
  fout << tokenLists.size() << endl;
  list<Tokens>::iterator it;
  for(it = tokenLists.begin(); it != tokenLists.end(); it++)
  {
    fout << it->tok << "\t" << it->lex << endl;
  }
  fout << endl;
}

void SyntaxAnalyzer::GrammarCheck(ofstream &fout)
{
  bool declarative;

  declarative = D(fout);
  if(declarative)
  {
    fout << "Yes. It's a declarative statement\n\n";
  }
  else
  {
    fout << "No, it's not a declarative statement\n\n";
    return;
  }
}

Tokens SyntaxAnalyzer::PopAndGetNextToken(ofstream &fout)
{
  //Pop the already examined element of the list
  tokenLists.pop_front();
  if(!IsEmpty())
  {
    //Examine the next element
    return tokenLists.front();
  }
  else
  {
    fout << "Can't pop an empty list\n";
    fout << "The program will be terminated\n\n";
    exit(5);
  }
}

/*******************************************************************************
 * The following methods D, DPrime, and Type are representing the productions:
 *  <D>       -> <Type> id <DPrime> ; | epsilon
 *  <DPrime>  -> , id <DPrime> | epsilon
 ******************************************************************************/
bool SyntaxAnalyzer::D(ofstream &fout)
{
  Tokens currToken;
  // bool   isDeclarative;
  string lex;
  string tok;

  // isDeclarative = false;
  currToken = tokenLists.front();
  lex = currToken.lex;
  tok = currToken.tok;
  if(Type(lex))
  {
    fout << "Token: " << tok << '\t' << "Lexeme: " << lex << endl;
    fout << "<Statement> -> <Declarative>" << endl;
    fout << "<Type> -> bool | float | int | double | string | char" << endl;
    //Pop the token <Type> of the list and get the next token
    currToken = PopAndGetNextToken(fout);
    lex = currToken.lex;
    tok = currToken.tok;
    if(tok == "Identifier")
    {
      fout << "Token: " << tok << '\t' << "Lexeme: " << lex << endl;
      //Pop the token <ID> of the list and get the next token
      currToken = PopAndGetNextToken(fout);
      lex = currToken.lex;
      tok = currToken.tok;
      if(DPrime(tok, lex, fout))
      {
        lex = tokenLists.front().lex;
        if(lex == ";")
        {
          tokenLists.pop_front();
          return true;
        }
      }
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

bool SyntaxAnalyzer::DPrime(string curTok, string curLex, ofstream &fout)
{
  Tokens currToken;
  string tok;
  string lex;
  if(curLex == ",")
  {
    fout << "Token: " << curTok << '\t' << "Lexeme: " << curLex << endl;
    fout << "<D_Prime> -> <,> <I> <D_Prime> | epsilon" << endl;
    currToken = PopAndGetNextToken(fout);
    lex = currToken.lex;
    tok = currToken.tok;
    if(tok == "Identifier")
    {
      fout << "Token: " << tok << '\t' << "Lexeme: " << lex << endl;
      currToken = PopAndGetNextToken(fout);
      lex = currToken.lex;
      tok = currToken.tok;
      return DPrime(tok, lex, fout);
    }
    else
    {
      fout << "Invalid syntax!\n";
      fout << "The program will be terminated\n\n";
      exit(5);
    }
  }
  else
  {
    return true;
  }
}

bool SyntaxAnalyzer::Type(string type)
{
  if(type == "bool" || type == "float" || type == "int" || type == "double" ||
     type == "string" || type == "char")
  {
    return true;
  }
  else
  {
    return false;
  }
}

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
 
