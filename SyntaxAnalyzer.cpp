#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer()
{
  flag = false;
}

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
  // bool declarative;

  // declarative = D(fout);
  if(D(fout))
  {
    fout << "It's a declarative statement\n\n";
  }
  else if(A(fout))
  {
    fout << "It's an assignment statement\n\n";
  }
  else
  {
    fout << "Idk what's wrong\n\n";
  }
  // fout << IsEmpty();
}

Tokens SyntaxAnalyzer::PopAndGetNextToken(ofstream &fout)
{
  //Pop the already examined element of the list
  // fout << "CUrrent : " << tokenLists.front().lex << endl;
  tokenLists.pop_front();
  // fout << IsEmpty() << endl;
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
    fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
    fout << "<Statement> -> <Declarative>" << endl;
    fout << "<Type> -> bool | float | int | double | string | char" << endl;
    //Pop the token <Type> of the list and get the next token
    currToken = PopAndGetNextToken(fout);
    lex = currToken.lex;
    tok = currToken.tok;
    if(tok == "Identifier")
    {
      fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
      //Pop the token <ID> of the list and get the next token
      currToken = PopAndGetNextToken(fout);
      lex = currToken.lex;
      tok = currToken.tok;
      if(DPrime(tok, lex, fout))
      {
        lex = tokenLists.front().lex;
        fout << "Here\n";
        if(lex == ";")
        {
          tokenLists.pop_front();
          return true;
        }
        else
        {
          fout << "Missing a semicolon. The program will be terminated\n";
          exit(5);
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
    fout << "Token: " << curTok << "\t" << "Lexeme: " << curLex << endl;
    fout << "<D_Prime> -> <,> <I> <D_Prime> | epsilon" << endl;
    currToken = PopAndGetNextToken(fout);
    lex = currToken.lex;
    tok = currToken.tok;
    if(tok == "Identifier")
    {
      fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
      currToken = PopAndGetNextToken(fout);
      lex = currToken.lex;
      tok = currToken.tok;
      return DPrime(tok, lex, fout);
    }
    else
    {
      fout << "Invalid syntax!\n";
      fout << "The program will be terminated\n\n";
      exit(2);
    }
  }
  else
  {
    fout << "DPrime -> epsilon.\n";
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
 * The following methods A representing the productions:
 *  <A>       -> id = <E> ;
 **************************************************************************/
bool SyntaxAnalyzer::A(ofstream &fout)
{
  Tokens currToken;
  string tok;
  string lex;

  // cout << "In A\n";
  currToken = tokenLists.front();
  tok = currToken.tok;
  lex = currToken.lex;
  // cout << tok << ", " << lex << endl;
  if(tok == "Identifier")
  {
    fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
    fout << "<Statement> -> <Assign>\n";
    fout << "<Assign> ->  <Identifier>  = <Expression>\n";
    //Pop the token <Identifier> of the list and get the next token
    currToken = PopAndGetNextToken(fout);
    tok = currToken.tok;
    lex = currToken.lex;
    if(lex == "=")
    {
      fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
      tokenLists.pop_front();
      if(E(fout))
      {
        currToken = tokenLists.front();
        tok = currToken.tok;
        lex = currToken.lex;
        if(lex == ";")
        {
          fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
          tokenLists.pop_front();
          return true;
        }
        else
        {
          fout << "Expect a ';'. The program will be terminated" << endl;;
          // return false;
          exit(5);
        }
      }
      else
      {
        fout << "Should include an Expression\n";
        exit(5);
      }
    }
    else
    {
      fout << "Should expect an equal sign\n";
      exit(5);
    }
  }
  //Not an assignment statement
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
bool SyntaxAnalyzer::E(ofstream &fout)
{
 Tokens currToken;
 string tok;
 string lex;

 currToken = tokenLists.front();
 tok = currToken.tok;
 lex = currToken.lex;

 fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
 fout << "<Expression> -> <Term> <Expression Prime>" << endl;

 if(T(fout))
 {
   if(EPrime(fout))
   {
     return true;
   }
 }
 else
 {
   return false;
 }
}

bool SyntaxAnalyzer::EPrime(ofstream &fout)
{
  Tokens currToken;
  string tok;
  string lex;

  currToken = tokenLists.front();
  tok = currToken.tok;
  lex = currToken.lex;
  if(lex == "+")
  {
    fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
    fout << "<EPrime> -> + <Term> <EPrime>" << endl;
    tokenLists.pop_front();
    if(T(fout))
    {
      return EPrime(fout);
    }
  }
  else if(lex == "-")
  {
    fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
    fout << "<EPrime> -> - <Term> <EPrime>" << endl;
    tokenLists.pop_front();
    if(T(fout))
    {
      return EPrime(fout);
    }
  }
  else
  {
    fout << "<EPrime> -> epsilon.\n";
    return true;
  }
}

bool SyntaxAnalyzer::T(ofstream &fout)
 {
   Tokens currToken;
   string tok;
   string lex;

   currToken = tokenLists.front();
   tok = currToken.tok;
   lex = currToken.lex;
   if(flag)
   {
     fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
   }
   flag = true;
   fout << "<Term> -> <Factor> <Term Prime>\n";
   if(F(fout))
   {
     if(TPrime(fout))
     {
       return true;
     }
   }
   else
   {
     return false;
   }
 }

bool SyntaxAnalyzer::TPrime(ofstream &fout)
{
  Tokens currToken;
  string tok;
  string lex;

  currToken = tokenLists.front();
  tok = currToken.tok;
  lex = currToken.lex;
  if(lex == "*")
  {
    fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
    fout << "<TPrime> -> * <Factor> <TPrime>" << endl;
    tokenLists.pop_front();
    if(F(fout))
    {
      return TPrime(fout);
    }
  }
  else if(lex == "/")
  {
    fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
    fout << "<TPrime> -> / <Factor> <TPrime>" << endl;
    tokenLists.pop_front();
    if(F(fout))
    {
      return TPrime(fout);
    }
  }
  else
  {
    fout << "<TPrime> -> epsilon.\n";
    return true;
  }
}

bool SyntaxAnalyzer::F(ofstream &fout)
{
  Tokens currToken;
  string tok;
  string lex;

  currToken = tokenLists.front();
  tok = currToken.tok;
  lex = currToken.lex;
  if(lex == "(")
  {
    fout << "<Factor> -> ( <Expression> )\n";
    tokenLists.pop_front();
    flag = false;
    //Revisit this part
    if(E(fout))
    {
      currToken = tokenLists.front();
      tok = currToken.tok;
      lex = currToken.lex;
      if(lex == ")")
      {
        fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
        tokenLists.pop_front();
        return true;
      }
      else
      {
        fout << "Should expect ')'\n";
        exit(5);
      }
    }
    else
    {
      fout << "Should expect an Expression\n";
      exit(5);
    }
  }
  else if(tok == "Identifier")
  {
    fout << "<Factor> -> <Identifier>\n";
    tokenLists.pop_front();
    return true;
  }
  else if(tok == "Integer" || tok == "Real")
  {
    fout << "<Factor> -> num\n";
    tokenLists.pop_front();
    return true;
  }
  else
  {
    fout << "Wrong syntax! The program will be terminated.\n\n";
    // return false;
    exit(5);
  }
}
