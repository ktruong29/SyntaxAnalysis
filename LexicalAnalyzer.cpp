#include "LexicalAnalyzer.h"


LexicalAnalyzer::LexicalAnalyzer(){}

LexicalAnalyzer::~LexicalAnalyzer(){}

bool LexicalAnalyzer::IsSeparator(char &c)
{
  return FindChar(c, SEPARATORS, NUM_SEPARATORS);
}

bool LexicalAnalyzer::IsOperator(char &c)
{
  return FindChar(c, OPERATORS, NUM_OPERATORS);
}

bool LexicalAnalyzer::IsKeyword(string &s)
{
  return FindString(s, KEYWORDS, NUM_KEYWORDS);
}

int LexicalAnalyzer::ConvertCharToCol(char &c)
{
  int col;

  if(isalpha(c)) //letter
  {
    col = 0;
  }
  else if(isdigit(c))
  {
    col = 1;
  }
  else if(IsOperator(c))
  {
    col = 2;
  }
  else if(IsSeparator(c) && c != '.')
  {
    col = 3;
  }
  else if(c == '!')
  {
    col = 4;
  }
  else if(c == '_')
  {
    col = 5;
  }
  else if(c == ' ')
  {
    col = 6;
  }
  else if(c == '.')
  {
    col = 7;
  }
  else if(c == '$')
  {
    col = 8;
  }
  //Other cases - special characters
  else
  {
    col = 9;
  }
  return col;
}

void LexicalAnalyzer::LexAnalyzer(string fileName, ofstream &fout)
{
  // index                          0  1  2  3  4  5  6  7  8  9
  //                               l  d  O  S  !  _ sp  .  $  Other
  const int STATE_TABLE[16][10] = {{1, 4, 8, 9, 10, 3, 3, 3, 3, 3},  //0
                                   {1, 1, 8, 2, 3, 1, 2, 3, 1, 2},   //1
                                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   //2
                                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   //3
                                   {3, 4, 5, 5, 3, 3, 5, 6, 3, 3},   //4
                                   {0, 0, 8, 0, 0, 0, 0, 0, 0, 0},   //5
                                   {3, 6, 7, 7, 3, 3, 7, 3, 3, 3},   //6
                                   {0, 0, 8, 0, 0, 0, 0, 0, 0, 0},   //7
                                   {1, 4, 12, 3, 3, 3, 13, 3, 3, 3},   //8
                                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   //9
                                   {10, 10, 10, 10, 11, 10, 10, 10, 10, 10},  //10
                                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},    //11
                                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},    //12
                                   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     //13
                                   {1, 4, 12, 3, 3, 3, 13, 3, 3, 3},  //14
                                   {1, 4, 12, 3, 3, 3, 13, 3, 3, 3}   //15
                                  };
  ifstream fin;
  char c;
  string lex;
  int row;
  int col;
  int state;
  // string op;
  string sep;
  // op  = "";
  sep = "";
  lex = "";
  row   = 0;
  state = 0;

  fin.open(fileName);
  if(fin.is_open())
  {
    while(!fin.eof())
    {
      fin.get(c);
      // cout << "New char: " << c << endl;
      // cout << "New lex: " << lex << endl;
      col = ConvertCharToCol(c);

      state = STATE_TABLE[row][col];
      // cout << "Current state: " << state << endl;
      switch(state)
      {
        case 0:
            break;
        //Appending a char into the string buffer

        case 1:
          if(!lex.empty() && IsOperator(lex[0]))
          {
            synAnalyzer.Push("Operator", lex);
            lex = "";
          }
          lex += c; // lex = lex + c   lex = int
            break;

        case 2:
          //Comparing keywords vs. identifiers
          if(IsKeyword(lex))
          {
            synAnalyzer.Push("Keyword", lex);
          }
          else
          {
            synAnalyzer.Push("Identifier", lex);
          }
          //Comparing the current character in the buffer (separator?)
          if(IsSeparator(c))
          {
            sep += c;
            synAnalyzer.Push("Separator", sep);
            sep = "";
          }
          //Clear the string buffer for the next input
          lex = "";
          state = 0;
            break;

        case 3: //Example: num%, num^
          // cout << "Unknow lexeme: " << lex << c << endl;
          if(c == '\n' && !synAnalyzer.IsEmpty())
          {
            // synAnalyzer.PrintAll(fout);
            synAnalyzer.GrammarCheck(fout);
          }
          lex = "";
          state = 0;
            break;

        //Appending digit into the string buffer
        case 4:
          if(!lex.empty() && IsOperator(lex[0]))
          {
            synAnalyzer.Push("Operator", lex);
            lex = "";
          }
          lex += c;
            break;

        case 5:
          synAnalyzer.Push("Integer", lex);
          lex = "";
          state = 0;
          //Comparing the current character in the buffer (operators or separators?)
          if(IsSeparator(c) && c != '.')
          {
            sep += c;
            synAnalyzer.Push("Separator", sep);
            sep = "";
            state = 0;
          }
          else if(IsOperator(c))
          {
            lex += c;
            state = 8;
          }
            break;

        //Appending digits
        case 6:
          if(!lex.empty() && IsOperator(lex[0]))
          {
            synAnalyzer.Push("Operator", lex);
            lex = "";
          }
          lex += c;
            break;

        //Real, float numbers
        case 7:
          synAnalyzer.Push("Real", lex);
          lex = "";
          state = 0;
          if(IsSeparator(c))
          {
            sep += c;
            synAnalyzer.Push("Separator", sep);
            sep = "";
            state = 0;
          }
          else if(IsOperator(c))
          {
            lex += c;
            state = 8;
          }
          lex = "";
          state = 0;
            break;

        case 8:
          if(!lex.empty())
          {
            if(IsKeyword(lex))
            {
              synAnalyzer.Push("Keyword", lex);
            }
            else
            {
              synAnalyzer.Push("Identifier", lex);
            }
            lex = "";
          }
          if(IsOperator(c))
          {
            lex += c;
          }
            break;

        case 9:
          sep += c;
          synAnalyzer.Push("Separator", sep);
          sep = "";
          state = 0;
            break;

        case 10:
            break;

        case 11:
          state = 0;
            break;

        case 12:
          lex += c;
          // fout << "COMPOUND OPERATOR\t\t" << "=\t\t" << lex << endl;
          synAnalyzer.Push("Compound Operator", lex);
          lex = "";
          state = 0;
            break;

        case 13:
          // fout << "OPERATOR\t\t" << "=\t\t" << lex << endl;
          synAnalyzer.Push("Operator", lex);
          lex = "";
          state = 0;
            break;

        case 14:
        if(!lex.empty())
        {
          // fout << "INTEGER\t\t" << "=\t\t" << lex << endl;
          synAnalyzer.Push("Integer", lex);
          lex = "";
        }
        if(IsOperator(c))
        {
          lex += c;
        }
          break;

        case 15:
        if(!lex.empty())
        {
          // fout << "REAL\t\t" << "=\t\t" << lex << endl;
          synAnalyzer.Push("Real", lex);
          lex = "";
        }
        if(IsOperator(c))
        {
          lex += c;
        }
          break;

       default:
          state = 0;
            break;
      }//END switch(state)

      row = state;
    }//END while(!fin.eof())
    //Print the symbol table
    synAnalyzer.PrintSymbolTable(fout);
    fin.close();
  }//END if(fin.is_open())
  else
  {
    cout << "Can't open " << fileName << endl;
  }
}



/*******************************************************************************
 * OTHER HELPER FUNCTIONS
 ******************************************************************************/
bool FindChar(char &c, const char CHAR_AR[], const int AR_SIZE)
{
  bool found;
  int  i;
  found = false;
  i = 0;

  while(!found && i < AR_SIZE)
  {
    //When an element is found, stops the iteration and returns 'found' back to the driver
    if(c == CHAR_AR[i])
    {
      found = true;
    }
    //Keeps incrementing i to access the next element in an array
    else
    {
      i++;
    }
  }//END while(!found && i < NUM_SEPARATORS)
  return found;
}

bool FindString(string &s, const string STRING_AR[], const int AR_SIZE)
{
 bool found;
 int  i;
 found = false;
 i = 0;

 while(!found && i < AR_SIZE)
 {
   //When an element is found, stops the iteration and returns 'found' back to the driver
   if(s == STRING_AR[i])
   {
     found = true;
   }
   //Keeps incrementing i to access the next element in an array
   else
   {
     i++;
   }
 }//END while(!found && i < NUM_SEPARATORS)
 return found;
}
