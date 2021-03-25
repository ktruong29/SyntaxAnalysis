#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(){}

SyntaxAnalyzer::~SyntaxAnalyzer(){}

void SyntaxAnalyzer::Push(string tok, string lex)
{
  Tokens currToken;
  currToken.lex = lex;
  currToken.tok = tok;
  // cout << "Lex: " << lex << " Token: " << tok << endl;
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

void SyntaxAnalyzer::PrintAll()
{
  cout << tokenLists.size() << endl;
  list<Tokens>::iterator it;
  for(it = tokenLists.begin(); it != tokenLists.end(); it++)
  {
    cout << it->tok << "\t" << it->lex << endl;
  }
}

void SyntaxAnalyzer::GrammarCheck()
{
  bool declarative;

  declarative = D();
  if(declarative)
  {
    cout << "Yes. It's a declarative statement\n\n";
  }
  else
  {
    cout << "No, it's not a declarative statement\n\n";
    return;
  }
}

bool SyntaxAnalyzer::D()
{
  Tokens currToken;
  bool   isDeclarative;
  string lex;
  string tok;

  isDeclarative = false;
  currToken = tokenLists.front();
  lex = currToken.lex;
  tok = currToken.tok;
  if(T(lex))
  {
    cout << "Token: " << tok << '\t' << "Lexeme: " << lex << endl;
    cout << "<Statement> -> <Declarative>" << endl;
    cout << "<Type> -> bool | float | int | double | string | char" << endl;

    currToken = PopAndGetNextToken();
    lex = currToken.lex;
    tok = currToken.tok;
    if(tok == "Identifier")
    {
      cout << "Token: " << tok << '\t' << "Lexeme: " << lex << endl;
      currToken = PopAndGetNextToken();
      if(currToken.lex == ";")
      {
        lex = currToken.lex;
        tok = currToken.tok;
        cout << "Token: " << tok << '\t' << "Lexeme: " << lex << endl;
        isDeclarative = true;
      }
    }
    else
    {
      cout << "Wrong syntax" << endl;
      return false;
    }
  }
  return isDeclarative;
}

bool SyntaxAnalyzer::T(string type)
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

Tokens SyntaxAnalyzer::PopAndGetNextToken()
{
  //Pop the already examined element of the list
  tokenLists.pop_front();
  //Examine the next element
  return tokenLists.front();
}
