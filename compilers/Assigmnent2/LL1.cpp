#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<map>
#include<stack>
#include"LexComment.h"
using namespace std;

typedef struct GrammerRules{
    string Left;
    vector<string> Right;
    vector<string> First;
    vector<string> Follow;
    bool FirstCalc;
    bool FollowCalc;
    bool Start;
}GrammerRules;

typedef struct PTEntry{
    string Left;
    string Right;
}PTEntry;

typedef struct ParsingTableKey{
    string Row;
    string Col;
}ParsingTableKey;


struct cmp_Func
	{
	   bool operator()(ParsingTableKey  O1, ParsingTableKey  O2)
	   {
	       if(O1.Row<O2.Row)
	       return true;
	       else if(O1.Row==O2.Row){
	           if(O1.Col<O2.Col)
	           return true;
	       }
	       else return false;
	      //return std::strcmp(a, b) < 0;
	   }
	};
//Global variables
vector<GrammerRules> RulesObj;
bool FindStart=false;
map<ParsingTableKey,PTEntry,cmp_Func> ParsingTable;
vector<string> NonTerminals;

vector<GrammerRules> LeftRecElimGra;
stack<string> ParsingStack;
vector<string> PrintParsingStack;    //for printing Stack

//vector<ParsingTable> LLTable;

//function decarations
void EnterGrammer(string line);
void CalculateFirst(GrammerRules *Obj);
void StringFirst(string str,bool *epsilon,vector<string> *First);
void EnterFollow(vector<string> * Follow,vector<string> First,bool *run);
//void CalculateFollow(GrammerRules *Obj);
void CalculateFollow();
void MakeTable();

string GiveValidNonTerminal();
void EliminateLeftRecursion();

void ParseGrammerSymbols(string Input,vector<string> *Output);
void ReadLexFile(string * Input);
void InputParsing(string Input);
void ReadSymbolTable(string *Input);
void PrintParsingSteps(string Inp);

bool checkDuplicates(vector<string> Vect,string Item);

void PrintGrammer();
void FindNonTerminals();

ofstream OutputFile;



int main()
{
    string line;

    OutputFile.open ("LL1Output.txt");



  //    ifstream Grammerfile("GrammerConf.txt");
    //  ifstream Grammerfile("InputFileGrammer.txt");

     //ifstream Grammerfile("Assign3Grammer.txt");
     ifstream Grammerfile("MastGrammer.txt");

   // ifstream Grammerfile("InputGrammerLL1.txt");
    if (Grammerfile.is_open())
    {
        while ( Grammerfile.good() )
        {
            getline (Grammerfile,line);
            EnterGrammer(line);
           // cout << line << endl;
        }
        Grammerfile.close();
    }
    else cout << "Unable to open Grammer File";

LeftRecElimGra=RulesObj;
FindNonTerminals();

EliminateLeftRecursion();
RulesObj=LeftRecElimGra;

PrintGrammer();


//calculating FIRST
       vector<GrammerRules>::iterator it;    //finding whether this non terminal has first calculated or not
                               for ( it=RulesObj.begin() ; it < RulesObj.end(); it++ )
                                {
                                    if(!(*it).FirstCalc)
                                    {
                                        CalculateFirst(&(*it));
                                    }

                                }

                                cout<<"FIRST PRINTING"<<endl;
//       vector<GrammerRules>::iterator it;    //finding whether this non terminal has first calculated or not
                               for ( it=RulesObj.begin() ; it < RulesObj.end(); it++ )
                                {
                                  cout<<(*it).Left<<" "<<(*it).FirstCalc<<" ";
                                  vector<string>::iterator it2;
                                  for ( it2=(*it).First.begin() ; it2 < (*it).First.end(); it2++ )
                                    {
                                        cout<<(*it2)<<" ";
                                    }
                                    cout<<endl;
                                }

                                cout<<"Calculating Follow"<<endl;

                                //calculating FOLLOW

                                for ( it=RulesObj.begin() ; it < RulesObj.end(); it++ )
                                {
                                    if((*it).Start)
                                    {
                                        (*it).Follow.push_back("$");    //applying 1st rule of follow
                                    }
                                    /*if(!(*it).FollowCalc)
                                    {
                                        CalculateFollow(&(*it));
                                    }*/

                                }
                                CalculateFollow();

                                 for ( it=RulesObj.begin() ; it < RulesObj.end(); it++ )
                                {
                                    (*it).FollowCalc=true;
                                }






                                cout<<"FOLLOW PRINTING"<<endl;
//       vector<GrammerRules>::iterator it;    //finding whether this non terminal has first calculated or not
                               for ( it=RulesObj.begin() ; it < RulesObj.end(); it++ )
                                {
                                  cout<<(*it).Left<<" "<<(*it).FollowCalc<<" ";
                                  vector<string>::iterator it2;
                                  for ( it2=(*it).Follow.begin() ; it2 < (*it).Follow.end(); it2++ )
                                    {
                                        cout<<(*it2)<<" ";
                                    }
                                    cout<<endl;
                                }

                                MakeTable();


                                cout<<"PRINTING LL1 TABLE"<<endl;
                                OutputFile<<"PRINTING LL1 TABLE"<<endl;

                                for( map<ParsingTableKey,PTEntry,cmp_Func>::iterator ii=ParsingTable.begin(); ii!=ParsingTable.end(); ++ii)
                                {
                                    cout << (*ii).first.Row <<" , "<<(*ii).first.Col<< " : " << (*ii).second.Left <<"-->"<<(*ii).second.Right<< endl;
                                    OutputFile<< (*ii).first.Row <<" , "<<(*ii).first.Col<< " : " << (*ii).second.Left <<"-->"<<(*ii).second.Right<< endl;
                                }


                                Main();        //calling Lexical analyzer, it will popolate symboltable
                              //  cout<<"LEXEMEVECTOR "<<lexemeVector.size()<<endl;

                               //  string Inp="~i*~i+~i$";

                                string Inp;
                                //ReadLexFile(&Inp);             //for reading lex tokens from file
                                ReadSymbolTable(&Inp); // Reading Tokens from Symbol table
                                cout<<"Input String"<<Inp<<endl;
                                InputParsing(Inp);

                                OutputFile.close();

                                return 0;
}

void EnterGrammer(string line)
{
    istringstream iss(line);
    string str[3];         // for left side, right side of grammer rule
    int i=0;
            do
            {
                //string sub;
                iss >> str[i];

                  //  cout << "Substring: " << i <<" "<< str[i] << endl;
                    i++;
            } while (iss);

            vector<GrammerRules>::iterator it;
            bool found =false;

            if(!str[0].compare(""))
            {
             //   cout<<"here";
            return;
            }

                for ( it=RulesObj.begin() ; it < RulesObj.end(); it++ )
                    {
                        if(!(*it).Left.compare(str[0]))
                        {
                            found=true;
                            (*it).Right.push_back(str[1]);

                        }
                    }
                    if(!found)
                    {
                        GrammerRules Obj;
                        Obj.Left.assign(str[0]);
                        Obj.Right.push_back(str[1]);
                        Obj.FirstCalc=false;
                        Obj.FollowCalc=false;
                        Obj.First.clear();
                        Obj.Follow.clear();

                        if(!FindStart)   //here assuming first line in Grammar.txt file is start symbol
                        {
                            Obj.Start=true;
                            FindStart=true;
                            ParsingStack.push("$");
                            PrintParsingStack.push_back("$");
                            ParsingStack.push(Obj.Left);         //putting start symbol of grammer on stack for parsing
                            PrintParsingStack.push_back(Obj.Left);
                        }
                        RulesObj.push_back(Obj);
                    }
}


void CalculateFirst(GrammerRules *Obj)
{
    vector<string>::iterator it;
    for ( it=Obj->Right.begin() ; it < Obj->Right.end(); it++ )    //considering rules one by one
                    {
                        if(Obj->Left.at(0)==(*it).at(0))      // if left side non terminal is equal to first character in right side rule , then skip the rule
                        {
                            continue;
                        }
                        string str;
                        str.assign((*it));
                        int i=0;
                        bool epsilon=false;
                        StringFirst(str,&epsilon,&(Obj->First));
                     /*   for(i=0;i<str.length();i++)
                        {

                        char ch=str.at(i);
                        if(ch>=65&&ch<=90)  // capital is first letter, so Non Terminal is in front
                        {

                            vector<GrammerRules>::iterator it1;    //finding whether this non terminal has first calculated or not
                               for ( it1=RulesObj.begin() ; it1 < RulesObj.end(); it1++ )
                                {
                                    if((*it1).Left.at(0)==ch)
                                    {
                                        if(!(*it1).FirstCalc){   //calculate by recursion
                                            CalculateFirst(&(*it1));
                                        }
                                        break;
                                    }
                                }

                                        vector<string>::iterator it2;
                                        bool localepsilon=false;

                                        for ( it2=(*it1).First.begin() ; it2 < (*it1).First.end(); it2++ )    // A->N1B putting everything in FIRST(N1) into FIRST(A),
                                        {
                                            if(!(*it2).compare("~"))
                                            {

                                                localepsilon=true;

                                            }else{
                                                Obj->First.push_back(*it2);
                                            }
                                        }

                                        if(localepsilon)
                                        {
                                            epsilon=true;
                                        }else{
                                            epsilon=false;
                                            break;
                                        }

                                }else{
                            if(ch=='~') //some special identifier or epsilon is there, special identitiefer s liek id are denoted by ~i
                            {
                                if(str.length()==1)   //case of epsilon(~)
                                {
                                    //Obj->First.push_back(str);
                                    epsilon=true;
                                    break;

                                }else if(str.length()>1){
                                    i++;
                                    char chs=str.at(i);
                                    string sid;
                                    sid.push_back(ch);
                                    sid.push_back(chs);
                                    Obj->First.push_back(sid);
                                    epsilon=false;
                                    break;
                                }

                            }else{   //simple terminal is there
                                string chs;
                                chs.push_back(ch);
                                Obj->First.push_back(chs);
                                epsilon=false;
                                break;
                                }
                        }
                    }*/

                    if(epsilon)
                    {
                       Obj->First.push_back("~");
                    }

                }
                     Obj->FirstCalc=true;
    }

/*void CalculateFollow(GrammerRules *Obj)
{
   // cout<<"Follow Cal "<<Obj->Left<<endl;
    vector<GrammerRules>::iterator it1;
    for ( it1=LeftRecElimGra.begin() ; it1 < LeftRecElimGra.end(); it1++ )   //searching for all non terminals
            {
                 vector<string>::iterator it2;
                 for ( it2=(*it1).Right.begin() ; it2 < (*it1).Right.end(); it2++ )    // searhig for all rules in all non terminals
                    {
                        int i=0,pos=0,found=0;
                        //for(i=0;i<(*it2).length();i++)
                        while(pos<(*it2).length())
                        {
                            found=(*it2).find(Obj->Left.c_str(),pos,1);
                            if (found!=string::npos)   //condition for found
                            {
                                pos=found+1;
                                if(pos<(*it2).length())      // checking for case A->(alpha)B(Beta)
                                {
                                string str=(*it2).substr(pos);
                                bool epsilon=false;
                                vector<string> First;
                                StringFirst(str,&epsilon,&First);

                                EnterFollow(&(Obj->Follow),First);


                                if(epsilon)   //case A->(alpha)B(Beta) FIRST((Beta)) contains epsilon
                                {
                                    if((*it1).Left.compare(Obj->Left))         //skipping rules of the form A->(alpha)A(beta)
                                    {
                                    if(!(*it1).FollowCalc)   //Follow not calculated
                                    {

                                        CalculateFollow(&(*it1));
                                    }
                                    EnterFollow(&(Obj->Follow),(*it1).Follow);
                                    }

                                }
                            }else{    //found on the last     case A->(alpha)B

                                 if((*it1).Left.compare(Obj->Left))    //skipping rules of the form A->(alpha)A
                                 {
                                 if(!(*it1).FollowCalc)   //Follow not calculated
                                    {
                                        CalculateFollow(&(*it1));
                                    }
                                    EnterFollow(&(Obj->Follow),(*it1).Follow);
                                 }


                                }
                                //char ch=(*it2).at(pos);

                            }else{
                                break;
                            }
                        }
                    }
                }
                Obj->FollowCalc=true;
}*/


void CalculateFollow()
{
   // cout<<"Follow Cal "<<Obj->Left<<endl;
   bool run =true;
   GrammerRules *Obj;
   vector<GrammerRules>::iterator it;
   vector<string>:: iterator it4;

   while(run)
   {
       run=false;
       cout<<"FALSE"<<endl;
       for ( it=RulesObj.begin() ; it < RulesObj.end(); it++ )
       {
           cout<<(*it).Left<<" "<<(*it).FollowCalc<<" ";

           for ( it4=(*it).Follow.begin() ; it4 < (*it).Follow.end(); it4++ )
                                    {
                                        cout<<(*it4)<<" ";
                                    }
                                    cout<<endl;




           Obj=&(*it);
           vector<GrammerRules>::iterator it1;
           for ( it1=LeftRecElimGra.begin() ; it1 < LeftRecElimGra.end(); it1++ )   //searching for all non terminals
            {
                vector<string>::iterator it2;
                for ( it2=(*it1).Right.begin() ; it2 < (*it1).Right.end(); it2++ )    // searhig for all rules in all non terminals
                {
                    int i=0,pos=0,found=0;
                        //for(i=0;i<(*it2).length();i++)
                        while(pos<(*it2).length())
                        {
                            found=(*it2).find(Obj->Left.c_str(),pos,1);
                            if (found!=string::npos)   //condition for found
                            {
                                pos=found+1;
                                if(pos<(*it2).length())      // checking for case A->(alpha)B(Beta)
                                {
                                string str=(*it2).substr(pos);
                                bool epsilon=false;
                                vector<string> First;

                                StringFirst(str,&epsilon,&First);

                                EnterFollow(&(Obj->Follow),First,&run);


                                if(epsilon)   //case A->(alpha)B(Beta) FIRST((Beta)) contains epsilon
                                {
                                    if((*it1).Left.compare(Obj->Left))         //skipping rules of the form A->(alpha)A(beta)
                                    {
                                    /*if(!(*it1).FollowCalc)   //Follow not calculated
                                    {

                                        CalculateFollow(&(*it1));
                                    }*/
                                    if((*it1).Follow.size()!=0)
                                    EnterFollow(&(Obj->Follow),(*it1).Follow,&run);
                                    }

                                }
                            }else{    //found on the last     case A->(alpha)B

                                 if((*it1).Left.compare(Obj->Left))    //skipping rules of the form A->(alpha)A
                                 {
                                 /*if(!(*it1).FollowCalc)   //Follow not calculated
                                    {
                                        CalculateFollow(&(*it1));
                                    }*/
                                     if((*it1).Follow.size()!=0)
                                    EnterFollow(&(Obj->Follow),(*it1).Follow,&run);
                                 }


                                }
                                //char ch=(*it2).at(pos);

                            }else{
                                break;
                            }
                        }
                    }
                }
                //Obj->FollowCalc=true;
   }
   }


}


void StringFirst(string str,bool *epsilon,vector<string> *First)
{

    //vector<string> First;
    int i=0;
    for(i=0;i<str.length();i++)
                        {

                        char ch=str.at(i);
                        if(ch>=65&&ch<=90)  // capital is first letter, so Non Terminal is in front
                        {

                            vector<GrammerRules>::iterator it1;    //finding whether this non terminal has first calculated or not
                               for ( it1=LeftRecElimGra.begin() ; it1 < LeftRecElimGra.end(); it1++ )
                                {
                                    if((*it1).Left.at(0)==ch)
                                    {
                                        if(!(*it1).FirstCalc){   //calculate by recursion
                                            CalculateFirst(&(*it1));
                                        }
                                        break;
                                    }
                                }

                                        vector<string>::iterator it2;
                                        bool localepsilon=false;

                                        for ( it2=(*it1).First.begin() ; it2 < (*it1).First.end(); it2++ )    // A->N1B putting everything in FIRST(N1) into FIRST(A),
                                        {
                                            if(!(*it2).compare("~"))
                                            {

                                                localepsilon=true;

                                            }else{
                                               // Obj->First.push_back(*it2);
                                               if(!checkDuplicates(*First,*it2))
                                                First->push_back(*it2);
                                            }
                                        }

                                        if(localepsilon)
                                        {
                                            *epsilon=true;
                                        }else{
                                            *epsilon=false;
                                            break;
                                        }

                                }else{
                            if(ch=='~') //some special identifier or epsilon is there, special identitiefer s liek id are denoted by ~i
                            {
                                if(str.length()==1)   //case of epsilon(~)
                                {
                                    //Obj->First.push_back(str);
                                    *epsilon=true;
                                    break;

                                }else if(str.length()>1){
                                    i++;
                                    char chs=str.at(i);
                                    string sid;
                                    sid.push_back(ch);
                                    sid.push_back(chs);
                                    if(!checkDuplicates(*First,sid))
                                    First->push_back(sid);
                                    *epsilon=false;
                                    break;
                                }

                            }else{   //simple terminal is there
                                string chs;
                                chs.push_back(ch);
                                if(!checkDuplicates(*First,chs))
                                First->push_back(chs);
                                *epsilon=false;
                                break;
                                }
                        }
                    }
}

bool checkDuplicates(vector<string> Vect,string Item)
{
    vector<string>::iterator it;
    bool found =false;
     for ( it=Vect.begin() ; it < Vect.end(); it++ )
     {
         if(!(*it).compare(Item))
         {
             found=true;
             break;
        }
     }
     return found;
}
void EnterFollow(vector<string> * Follow,vector<string> First,bool * run)
{
    vector<string>::iterator it3;
    for ( it3=First.begin() ; it3 < First.end(); it3++ )
                                {
                                    bool found=false;
                                    vector<string>::iterator it4;
                                    for ( it4=Follow->begin() ; it4 < Follow->end(); it4++ )         // checking for duplicates
                                    {
                                        if(!(*it3).compare(*it4))
                                        {
                                            found=true;
                                            break;
                                        }
                                    }
                                    if(!found)
                                    {
                                        (*Follow).push_back(*it3);
                                        *run=true;
                                    }
                                }
}


void MakeTable()
{
      vector<GrammerRules>::iterator it1;
    for ( it1=RulesObj.begin() ; it1 < RulesObj.end(); it1++ )
            {
                vector<string>::iterator it2;
                for ( it2=(*it1).Right.begin() ; it2 < (*it1).Right.end(); it2++ )
                {
                    bool epsilon=false;
                    vector<string>  Vect;
                    StringFirst((*it2),&epsilon,&Vect);
                    vector<string>::iterator it3;


                    for ( it3=Vect.begin() ; it3 < Vect.end(); it3++ )    //applying Rules 1 Of book
                    {
                        pair<map<ParsingTableKey,PTEntry,cmp_Func>::iterator,bool> ret;

                        ParsingTableKey Key;
                        PTEntry Entry;
                        Key.Row.assign((*it1).Left);
                        Key.Col.assign(*it3);

                        Entry.Left.assign((*it1).Left);
                        Entry.Right.assign(*it2);

                        ret=ParsingTable.insert(pair<ParsingTableKey,PTEntry>(Key,Entry));
                        if(!ret.second)
                        {
                            cout<<"Duplicate Entry"<<endl;
                        }
                    }

                    if(epsilon)          //appying Rule 2
                    {
                         for ( it3=(*it1).Follow.begin() ; it3 < (*it1).Follow.end(); it3++ )
                        {
                        pair<map<ParsingTableKey,PTEntry,cmp_Func>::iterator,bool> ret;

                        ParsingTableKey Key;
                        PTEntry Entry;
                        Key.Row.assign((*it1).Left);
                        Key.Col.assign(*it3);

                        Entry.Left.assign((*it1).Left);
                        Entry.Right.assign(*it2);

                        ret=ParsingTable.insert(pair<ParsingTableKey,PTEntry>(Key,Entry));
                            if(!ret.second)
                            {
                                cout<<"Duplicate Entry"<<endl;
                            }
                        }

                    }
                }
            }
}



void EliminateLeftRecursion()
{
    //vector<GrammerRules>::iterator it;
    int i=0;
    while(i<LeftRecElimGra.size())

    //for ( it=LeftRecElimGra.begin() ; it < LeftRecElimGra.end(); it++ )
        {
            vector<string> LeftRec,NotLeftRec;
            vector<string>::iterator it1;
            string NewNonTerm= GiveValidNonTerminal();
            for ( it1=(LeftRecElimGra.at(i)).Right.begin() ; it1 < (LeftRecElimGra.at(i)).Right.end(); it1++ )
                {
                    if((LeftRecElimGra.at(i)).Left.at(0)==(*it1).at(0))
                    {
                        string str=(*it1).substr(1);   //leaving first symbol(A)
                        str.append(NewNonTerm);
                        LeftRec.push_back(str);
                    }else{
                        string str=(*it1);
                        str.append(NewNonTerm);

                        NotLeftRec.push_back(str);
                    }
                }

                vector<string> NewProd;
                if(LeftRec.size()>=1)
                {
                    (LeftRecElimGra.at(i)).Right=NotLeftRec;
                    GrammerRules Obj;
                    Obj.Left=NewNonTerm;
                    Obj.Right=LeftRec;
                    Obj.Right.push_back("~");      //inputting epsilon
                    Obj.FirstCalc=false;
                    Obj.FollowCalc=false;
                    Obj.Start=false;
                    Obj.First.clear();
                    Obj.Follow.clear();

                    NonTerminals.push_back(NewNonTerm);       //entering new on term into our vector
                    LeftRecElimGra.push_back(Obj);
                }
                i++;
        }
}

string GiveValidNonTerminal()
{
    int i=65;        // 65 for A
    for(i=65;i<=90;i++)  //checking which A-Z terminal is npt there in the grammer to assign it during Eliminating lEft Recursuion
    {
        bool found=false;
        vector<string>::iterator it1;
        for ( it1=NonTerminals.begin() ; it1 < NonTerminals.end(); it1++ )
        {
            if((*it1).at(0)==i)
                {
                    found=true;
                    break;
                }
        }
        if(!found)
        {
            char ch=i;
            string str;
            str.append(1,ch);
            return str;
        }
    }
    cout<<"ERROR "<<endl;
    return "";
}


void InputParsing(string Input)
{
    int i=0;
    bool Fail=false;
    while(ParsingStack.top().compare("$"))  //X!=$
    {
        PrintParsingSteps(Input.substr(i));
        char ch,ch2;
        ch=Input.at(i);
        string str;
        str.append(1,ch);
        string X=ParsingStack.top();
        if(ch=='~')
        {
            if((i+1)<Input.length())
            {
                ch2=Input.at(i+1);
                str.append(1,ch2);
            }else{
                cout<<"ERROR in Parsing";
                break;

            }

        }


        if(!str.compare(X))
        {
          //  cout<<"SAME"<<ParsingStack.top()<<endl;
            ParsingStack.pop();
            PrintParsingStack.pop_back();
            //cout<<"TOP"<<ParsingStack.top()<<endl;

            i+=str.length();
        }else if (!( X.at(0)>=65 && X.at(0)<=90 )){   //if terminal then error
            cout<<"Error: Parsing will now halt "<<i<<endl;
            Fail=true;
            break;
        }else {
            map<ParsingTableKey,PTEntry,cmp_Func>::iterator it;
                 ParsingTableKey Key;
                 Key.Row=X;

                 Key.Col=str;
                 it=ParsingTable.find(Key);

                // cout<<"Key Row"<<Key.Row<<" Col "<<Key.Col<<endl;

                 if(it==ParsingTable.end())
                 {
                     cout<<"WRONG STRING"<<endl;
                     Fail=true;
                     break;
                 }else{
                     string Left=(*it).second.Left;
                     string Right=(*it).second.Right;
                     ParsingStack.pop();
                     PrintParsingStack.pop_back();
                     vector<string> Items;
                     Items.clear();
                     ParseGrammerSymbols(Right,&Items);
                     int size=Items.size();
                     size--;   //making index
                    // cout<<"Items"<<endl;
                     while(size>=0)
                     {
                         ParsingStack.push(Items.at(size));
                         PrintParsingStack.push_back(Items.at(size));
                       //  cout<<Items.at(size)<<endl;
                         size--;
                     }
                }
            }
        }
        if(!Fail)
        {
            cout<<"Successful String Accepted"<<endl;
        }
}



void ReadLexFile(string * Input)
{
     ifstream LexFile ("LexFileLL1.txt");
     string line;
     if (LexFile.is_open())
    {
        while ( LexFile.good() )
        {
        getline (LexFile,line);
        //cout << line << endl;
        Input->append(line);
        }
        Input->append("$");
        LexFile.close();
    }


  else cout << "Unable to open file";
}


void ParseGrammerSymbols(string Input,vector<string> *Output)
{
    int i=0;
    while(i<Input.length())
    {
        char ch,ch2;
        ch=Input.at(i);
        string str;
        str.clear();
        i++;
        str.append(1,ch);
        if(ch=='~')
        {
            if(Input.length()==1)   //epsilon, no need to push
            {
                return;
            }else{
                ch2=Input.at(i);
                i++;
                str.append(1,ch2);

                Output->push_back(str);

                }
        }else{
            Output->push_back(str);
        }
    }
}

void ReadSymbolTable(string *Input)
{
    vector<SymbolTableStruct>::iterator it;
	for (it=NewSymbolTable.begin();it!=NewSymbolTable.end();it++)
	{
        Input->append((*it).Token);
    }
    Input->append("$");
}

void PrintParsingSteps(string Inp)
{
    vector<string>::iterator it;
    for (it=PrintParsingStack.begin();it!=PrintParsingStack.end();it++)
	{
        cout<<(*it)<<" ";
        OutputFile<<(*it)<<" ";

    }
    cout<<"                     ";
    OutputFile<<"                     ";
    cout<<Inp<<endl;
    OutputFile<<Inp<<endl;

}


void PrintGrammer()
{
    //printing Grammer
    cout<<"PRINTING ELIMINATED LEFT RECURISVE GRAMMER"<<endl;       //no need for elimiating left recursion
    vector<GrammerRules>::iterator It;
    for ( It=LeftRecElimGra.begin() ; It < LeftRecElimGra.end(); It++ )
    {
        //cout<<(*It).Left<<"-->"<<;
        vector<string>::iterator it1;
        for ( it1=(*It).Right.begin() ; it1 < (*It).Right.end(); it1++ )
        {
            cout<<(*It).Left<<" --> "<<*it1<<endl;
        }
    }
}

void FindNonTerminals()
{
    vector<GrammerRules>::iterator it;
    for ( it=LeftRecElimGra.begin() ; it < LeftRecElimGra.end(); it++ )
    {
        bool found=false;
        vector<string>::iterator it1;
        for ( it1=NonTerminals.begin() ; it1 < NonTerminals.end(); it1++ )
        {
            if(!(*it).Left.compare((*it1)))
            {
                found=true;
                break;
            }
        }
        if(!found)
        {
            NonTerminals.push_back((*it).Left);

        }
    }
}
