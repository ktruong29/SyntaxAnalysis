#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer()
{
  flag      = false;
  whileLoop = false;
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
else if(W(fout))
{
  fout << "It's a while loop\n\n";
}
else if(I(fout))
{
  fout << "It's an if statement\n\n";
}
else if(B(fout))
{
  fout << "It's a begin statement\n\n";
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
  // fout <<"here\n";
  //Examine the next element
  return tokenLists.front();
}
else
{
  cout << "Can't pop an empty list\n";
  cout << "The program will be terminated\n\n";
  exit(EXIT_FAILURE);
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
  bool   isD;
  string lex;
  string tok;

  isD = false;
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
        // fout << "Here\n";
        if(lex == ";")
        {
          fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
          tokenLists.pop_front();
          isD = true;
        }
        else
        {
          cout << "The next token should be a semicolon. The program will be terminated\n";
          exit(EXIT_FAILURE);
        }
      }
    }
    else
    {
      cout << "Missing an identifier. The program will be terminated\n";
      exit(EXIT_FAILURE);
    }
  }
  return isD;
}

bool SyntaxAnalyzer::DPrime(string curTok, string curLex, ofstream &fout)
{
  Tokens currToken;
  string tok;
  string lex;
  bool isDPrime;

  isDPrime = false;

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
      cout << "Invalid syntax! Should expect an identifier\n";
      cout << "The program will be terminated\n\n";
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    fout << "DPrime -> epsilon.\n";
    isDPrime = true;
  }
  return isDPrime;
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
  bool isA;

  isA = false;
  // cout << "In A\n";
  currToken = tokenLists.front();
  tok = currToken.tok;
  lex = currToken.lex;
  // cout << tok << ", " << lex << endl;
  // fout << "Here\n";
  if(tok == "Identifier")
  {
    fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
    fout << "<Statement> -> <Assign>\n";
    fout << "<Assign> ->  <Identifier>  = <Expression>\n";
    //Pop the token <Identifier> of the list and get the next token
    currToken = PopAndGetNextToken(fout);
    tok = currToken.tok;
    lex = currToken.lex;
    // fout << "test " << lex << endl;
    if(lex == "=")
    {
      fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
      tokenLists.pop_front();
      // fout << IsEmpty() << endl;
      if(E(fout))
      {
        // lex = "MISSING_SEMICOLON";
        if(!IsEmpty())
        {
          currToken = tokenLists.front();
          tok = currToken.tok;
          lex = currToken.lex;
        }
        if(lex == ";")
        {
          fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
          tokenLists.pop_front();
          isA = true;
        }
        else if(whileLoop)
        {
          isA = true;
        }
        else
        {
          cout << "Expect a ';'. The program will be terminated" << endl;;
          // return false;
          exit(EXIT_FAILURE);
        }
      }
      else
      {
        fout << "Should include an Expression\n";
        exit(EXIT_FAILURE);
      }
    }
    else
    {
      fout << "Should expect an equal sign\n";
      exit(1);
    }
  }
  //Not an assignment statement
  else
  {
    isA = false;
  }
  return isA;
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
  bool   isE;

  isE = false;
  lex = "EMPTY";
  if(!IsEmpty())
  {
   currToken = tokenLists.front();
   tok = currToken.tok;
   lex = currToken.lex;
  }
  // currToken = tokenLists.front();
  // tok = currToken.tok;
  // lex = currToken.lex;

  fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
  fout << "<Expression> -> <Term> <Expression Prime>" << endl;

  if(T(fout))
  {
   if(EPrime(fout))
   {
     isE = true;
   }
  }
  else
  {
   isE = false;
  }
  return isE;
  }

  bool SyntaxAnalyzer::EPrime(ofstream &fout)
  {
  Tokens currToken;
  string tok;
  string lex;
  bool   isEPrime;

  isEPrime = false;
  lex = "EMPTY";
  if(!IsEmpty())
  {
    currToken = tokenLists.front();
    tok = currToken.tok;
    lex = currToken.lex;
  }
  // currToken = tokenLists.front();
  // tok = currToken.tok;
  // lex = currToken.lex;
  if(lex == "+")
  {
    fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
    fout << "<EPrime> -> + <Term> <EPrime>" << endl;
    tokenLists.pop_front();
    flag = true;
    if(T(fout))
    {
      flag = false;
      return EPrime(fout);
    }
  }
  else if(lex == "-")
  {
    fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
    fout << "<EPrime> -> - <Term> <EPrime>" << endl;
    tokenLists.pop_front();
    flag = true;
    if(T(fout))
    {
      flag = false;
      return EPrime(fout);
    }
  }
  else
  {
    fout << "<EPrime> -> epsilon.\n";
    isEPrime = true;
  }
  return isEPrime;
}

bool SyntaxAnalyzer::T(ofstream &fout)
{
  Tokens currToken;
  string tok;
  string lex;
  bool   isT;

  isT = false;
  lex = "EMPTY";
  if(!IsEmpty())
  {
   currToken = tokenLists.front();
   tok = currToken.tok;
   lex = currToken.lex;
  }
  // currToken = tokenLists.front();
  // tok = currToken.tok;
  // lex = currToken.lex;
  if(flag)
  {
   fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
  }
  // flag = true;
  fout << "<Term> -> <Factor> <Term Prime>\n";
  if(F(fout))
  {
   if(TPrime(fout))
   {
     isT = true;
   }
  }
  else
  {
   isT = false;
  }
  return isT;
}

bool SyntaxAnalyzer::TPrime(ofstream &fout)
{
  Tokens currToken;
  string tok;
  string lex;
  bool   isTPrime;

  isTPrime = false;
  lex = "EMPTY";
  if(!IsEmpty())
  {
    currToken = tokenLists.front();
    tok = currToken.tok;
    lex = currToken.lex;
  }
  if(lex == "*")
  {
    fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
    fout << "<TPrime> -> * <Factor> <TPrime>" << endl;
    tokenLists.pop_front();
    flag = true;
    if(F(fout))
    {
      flag = false;
      return TPrime(fout);
    }
  }
  else if(lex == "/")
  {
    fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
    fout << "<TPrime> -> / <Factor> <TPrime>" << endl;
    tokenLists.pop_front();
    flag = true;
    if(F(fout))
    {
      flag = false;
      return TPrime(fout);
    }
  }
  else
  {
    fout << "<TPrime> -> epsilon.\n";
    isTPrime = true;
  }
  return isTPrime;
}

bool SyntaxAnalyzer::F(ofstream &fout)
{
  Tokens currToken;
  string tok;
  string lex;
  bool   isF;

  isF = false;
  lex = "EMPTY";
  if(!IsEmpty())
  {
    currToken = tokenLists.front();
    tok = currToken.tok;
    lex = currToken.lex;
  }
  // currToken = tokenLists.front();
  // tok = currToken.tok;
  // lex = currToken.lex;
  if(lex == "(")
  {
    if(flag)
    {
      fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
    }
    // fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
    fout << "<Factor> -> ( <Expression> )\n";
    tokenLists.pop_front();
    flag = false;
    //Revisit this part
    if(E(fout))
    {
      lex = "EMPTY";
      if(!IsEmpty())
      {
        currToken = tokenLists.front();
        tok = currToken.tok;
        lex = currToken.lex;
      }
      // currToken = tokenLists.front();
      // tok = currToken.tok;
      // lex = currToken.lex;
      if(lex == ")")
      {
        fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
        tokenLists.pop_front();
        isF = true;
      }
      else
      {
        cout << "Should expect ')'\n";
        exit(EXIT_FAILURE);
      }
    }
    else
    {
      cout << "Should expect an Expression\n";
      exit(EXIT_FAILURE);
    }
  }
  else if(tok == "Identifier")
  {
    if(flag)
    {
      fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
    }
    // fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
    fout << "<Factor> -> <Identifier>\n";
    tokenLists.pop_front();
    isF = true;
  }
  else if(tok == "Integer" || tok == "Real")
  {
    if(flag)
    {
      fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
    }
    // fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
    fout << "<Factor> -> num\n";
    tokenLists.pop_front();
    isF =  true;
  }
  else
  {
    fout << "Wrong syntax for F! The program will be terminated.\n\n";
    // return false;
    // exit(5);
    isF = false;
  }
  return isF;
}

/***************************************************************************
* The following method S (Statement) represents the productions:
*  <S>       -> <A> | <D> | <W> ;  W - while loop
**************************************************************************/
bool SyntaxAnalyzer::S(ofstream &fout)
{
 bool isS;
 isS = false;
 whileLoop = true;
 if(A(fout) || D(fout) || W(fout) || I(fout) || B(fout))
 {
   isS = true;
 }
 whileLoop = false;
 return isS;
}

/***************************************************************************
* The following methods W, C, and R represent the productions:
*  <W>      -> 'while' <C> 'do' <S> 'whileend' ;
*  <C>      -> <E> <R> <E> | <E>
*  <R>      -> < | >
**************************************************************************/
bool SyntaxAnalyzer::W(ofstream &fout)
{
  Tokens currToken;
  string lex;
  string tok;
  bool   isW;

  isW = false;
  //Debugging - if the list is empty, then lex will have string value of "EMPTY"
  //If the list is not empty -> lex will be substitute with the current lexeme
  lex = "EMPTY";
  if(!IsEmpty())
  {
    currToken = tokenLists.front();
    tok = currToken.tok;
    lex = currToken.lex;
  }
  if(lex == "while")
  {
    fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
    fout << "while <C> do <S> whileend ;\n";
    tokenLists.pop_front();
    if(C(fout))
    {
      //Debugging - if the list is empty, then lex will have string value of "EMPTY"
      //If the list is not empty -> lex will be substitute with the current lexeme
      lex = "EMPTY";
      if(!IsEmpty())
      {
        currToken = tokenLists.front();
        tok = currToken.tok;
        lex = currToken.lex;
      }
      if(lex == "do")
      {
        fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
        tokenLists.pop_front();
        if(S(fout))
        {
          //Debugging - if the list is empty, then lex will have string value of "EMPTY"
          //If the list is not empty -> lex will be substitute with the current lexeme
          lex = "EMPTY";
          if(!IsEmpty())
          {
            currToken = tokenLists.front();
            tok = currToken.tok;
            lex = currToken.lex;
          }
          if(lex == "whileend")
          {
            fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
            tokenLists.pop_front();
            //Debugging - if the list is empty, then lex will have string value of "EMPTY"
            //If the list is not empty -> lex will be substitute with the current lexeme
            // lex = "EMPTY";
            if(!IsEmpty())
            {
              currToken = tokenLists.front();
              tok = currToken.tok;
              lex = currToken.lex;
            }
            if(lex == ";")
            {
              fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
              tokenLists.pop_front();
              isW = true;
            }
            else
            {
              cout << "Should expect a ;. The program will be terminated.\n";
              exit(EXIT_FAILURE);
            }
          }
          else
          {
            cout << "Should expect 'whileend'. The program will be terminated.\n";
            exit(EXIT_FAILURE);
          }
        }
      }
      else
      {
        cout << "Should expect 'do' ;. The program will be terminated.\n";
        exit(EXIT_FAILURE);
      }
    }
  }
  return isW;
}

bool SyntaxAnalyzer::C(ofstream &fout)
{
  bool isC;

  isC = false;
  if(E(fout))
  {
    if(CPrime(fout))
    {
      isC = true;
    }
  }
  return isC;
}

bool SyntaxAnalyzer::CPrime(ofstream &fout)
{
  bool isCPrime;

  isCPrime = false;
  if(R(fout))
  {
    if(E(fout))
    {
      isCPrime = true;
    }
  }
  else
  {
    isCPrime = true;
  }
  return isCPrime;
}

bool SyntaxAnalyzer::R(ofstream &fout)
{
  Tokens currToken;
  string lex;
  string tok;
  bool   isR;

  isR = false;
  //Debugging - if the list is empty, then lex will have string value of "EMPTY"
  //If the list is not empty -> lex will be substitute with the current lexeme
  lex = "EMPTY";
  if(!IsEmpty())
  {
    currToken = tokenLists.front();
    tok = currToken.tok;
    lex = currToken.lex;
  }
  if(lex == "<" || lex == ">" || lex == ">=" || lex == "<=" || lex == "<>" || lex == "==")
  {
    fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
    fout << "<R> -> < | > | >= | <= | <> | ==\n";
    tokenLists.pop_front();
    isR = true;
  }
  //If incorrect compound operators (e.g. >< )
  else if(tok == "Compound Operator")
  {
    cout << "Incorrect compound operator. The program will be terminated.\n";
    exit(EXIT_FAILURE);
  }
  return isR;
}

/***************************************************************************
 * The following method I represent the productions:
 *  <I> -> 'if' <C> 'then' <S> 'else' <S> 'endif';
 *  <C>      -> <E> <CPrime>
 *  <CPrime> -> <R> <E> | epsilon
 *  <A>       -> id = <E> ;
 **************************************************************************/
bool SyntaxAnalyzer::I(ofstream &fout)
{
  Tokens currToken;
  string lex;
  string tok;
  bool   isI;

  isI = false;
  lex = "EMPTY";
  if(!IsEmpty())
  {
    currToken = tokenLists.front();
    tok = currToken.tok;
    lex = currToken.lex;
  }
  if(lex == "if")
  {
    fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
    tokenLists.pop_front();
    if(C(fout))
    {
      lex = "EMPTY";
      if(!IsEmpty())
      {
        currToken = tokenLists.front();
        tok = currToken.tok;
        lex = currToken.lex;
      }
      if(lex == "then")
      {
        fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
        tokenLists.pop_front();
        if(S(fout))
        {
          lex = "EMPTY";
          if(!IsEmpty())
          {
            currToken = tokenLists.front();
            tok = currToken.tok;
            lex = currToken.lex;
          }
          if(lex == "else")
          {
            fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
            tokenLists.pop_front();
            if(S(fout))
            {
              lex = "EMPTY";
              if(!IsEmpty())
              {
                currToken = tokenLists.front();
                tok = currToken.tok;
                lex = currToken.lex;
              }
              if(lex == "endif")
              {
                fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
                tokenLists.pop_front();
                lex = "EMPTY";
                if(!IsEmpty())
                {
                  currToken = tokenLists.front();
                  tok = currToken.tok;
                  lex = currToken.lex;
                }
                if(lex == ";")
                {
                  fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
                  tokenLists.pop_front();
                  isI = true;
                }
                else
                {
                  cout << "Should expect a ;. The program will be terminated.\n";
                  exit(EXIT_FAILURE);
                }
              }
              else
              {
                cout << "Should expect an 'endif' keyword. The program will be terminated.\n";
                exit(EXIT_FAILURE);
              }
            }
          }
          else
          {
            cout << "Should expect an 'else' keyword. The program will be terminated.\n";
            exit(EXIT_FAILURE);
          }
        }
      }
      else
      {
        cout << "Should expect a 'then' keyword. The program will be terminated.\n";
        exit(EXIT_FAILURE);
      }
    }
  }
  else
  {
    isI = false;
  }
  return isI;
}

bool SyntaxAnalyzer::B(ofstream &fout)
{
  Tokens currToken;
  string lex;
  string tok;
  bool   isF;

  isF = false;
  lex = "EMPTY";
  if(!IsEmpty())
  {
    currToken = tokenLists.front();
    tok = currToken.tok;
    lex = currToken.lex;
  }
  if(lex == "begin")
  {
    fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
    fout << "begin <S> <MS> end ;\n";
    tokenLists.pop_front();
    // fout << "Here1\n";
    if(S(fout))
    {
      // fout << "Here2\n";
      if(MS(fout))
      {
        lex = "EMPTY";
        if(!IsEmpty())
        {
          currToken = tokenLists.front();
          tok = currToken.tok;
          lex = currToken.lex;
        }
        if(lex == "end")
        {
          fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
          tokenLists.pop_front();
          lex = "EMPTY";
          if(!IsEmpty())
          {
            currToken = tokenLists.front();
            tok = currToken.tok;
            lex = currToken.lex;
          }
          if(lex == ";")
          {
            fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
            tokenLists.pop_front();
            isF = true;
          }
          else
          {
            cout << "Should expect a semicolon.\n";
            exit(EXIT_FAILURE);
          }
        }
        else
        {
          cout << "Should expect an 'end'.\n";
          exit(EXIT_FAILURE);
        }
      }
      else
      {
        cout << "Shoud expect a more statement.\n";
        exit(EXIT_FAILURE);
      }
    }
    else
    {
      cout << "Should expect a statement.\n";
      exit(EXIT_FAILURE);
    }
  }
  return isF;
}

bool SyntaxAnalyzer::MS(ofstream &fout)
{
  Tokens currToken;
  string lex;
  string tok;
  bool   isMS;

  // isMS = false;
  // lex = "EMPTY";
  // if(!IsEmpty())
  // {
  //   currToken = tokenLists.front();
  //   tok = currToken.tok;
  //   lex = currToken.lex;
  // }
  // if(lex == ";")
  // {
  //   fout << "Token: " << tok << "\t" << "Lexeme: " << lex << endl;
  //   fout << "<MS> -> ; <S> <MS>\n";
  //   tokenLists.pop_front();
    if(S(fout))
    {
      if(MS(fout))
      {
        isMS = true;
      }
    }
  // }
  else
  {
    fout << "<MS> -> epsilon\n";
    isMS = true;
  }
  return isMS;
}
