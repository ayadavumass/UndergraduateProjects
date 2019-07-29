#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<map>
#include<stack>
#include<stdlib.h>
#include"LexComment.h"
using namespace std;

typedef struct GrammerRules{
    string Left;
    vector<string> Right;
    vector<string> Follow;
     vector<string> First;
      bool FirstCalc;
    bool FollowCalc;
    bool Start;
}GrammerRules;

typedef struct Items{
    string Left;
    string Right;
    int DotPos;
}Items;

typedef struct GotoInfo{
    int State;
    string Symbol;
}GotoInfo;

typedef struct LR0Items{
    vector<Items> StateItems;
    int State;
    vector<GotoInfo> Goto;
}LR0Items;

typedef struct Processing{
    string DotSymbol;
    vector<Items> ProItems;
    vector<Items> Goto;
}Processing;

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

typedef struct PTEntry{
    bool s_r;             //true for shift, false for reduce
    bool A_G;             //true for action, false fo Goto
    string Left;
    string Right;
    int State;
    bool Accept;            //true for accept, false for not accept;
}PTEntry;







//Global Variables
bool FindStart=false;
vector<GrammerRules> RulesObj;
vector<GrammerRules> LeftRecElimGra;
vector<string> NonTerminals;            //stores distinct Nonterminals of the grammer for eliminating Left Recursion
vector<LR0Items> CanItems;
int NumberOfStates=0;            //indicates next state at time of computing canonical LR0 item, and finally number of states
map<ParsingTableKey,PTEntry,cmp_Func> ParsingTable;
stack<string> ParsingStack;
vector<string> PrintParsingStack;

//Function Declarations
void EnterGrammer(string line);
void Closure(vector<Items> Input,vector<Items> * Output);
void EnterIntoClosure(GrammerRules Obj,vector<Items> *Output,bool *StopFlag);
void GOTO(vector<Items> Input,string X,vector<Items> * Output);
void ComputeCanonicalItems();
bool vectorCompare(vector<Items> Vect1,vector<Items> Vect2);
void PrintItems(vector<Items> Item);

void CalculateFollow(GrammerRules *Obj);
void StringFirst(string str,bool *epsilon,vector<string> *First);
void EnterFollow(vector<string> * Follow,vector<string> First);
void CalculateFirst(GrammerRules *Obj);

void FindNonTerminals();
string GiveValidNonTerminal();
void EliminateLeftRecursion();
void ReturnFollow(string Left,vector<string> * Output);
bool InsertParsingTable(ParsingTableKey Key,PTEntry Entry);
bool ifStart(string Left);
void ReadSymbolTable(string *Input);

void MakeTable();


void InputParsing(string Input);
int calculateEffectiveLength(string str);

bool checkDuplicates(vector<string> Vect,string Item);


void ReadLexFile(string * Input);


void PrintParsingSteps(string Inp);


int main()
{
    string line;

    //ifstream Grammerfile("GrammerConf.txt");
   // ifstream Grammerfile("GrammerSLR.txt");
   ifstream Grammerfile("Assign3Grammer.txt");
    //ifstream Grammerfile("InputFileGrammer.txt");
//    ifstream Grammerfile("ConflictGrammer2.txt");

    if (Grammerfile.is_open())
    {
        while ( Grammerfile.good() )
        {
            getline (Grammerfile,line);
            EnterGrammer(line);
          //  cout << line << endl;
        }
        Grammerfile.close();
    }
else cout << "Unable to open Grammer File";

LeftRecElimGra=RulesObj;         //copying same Grammer Rules twice for Eliminating Left recursion

FindNonTerminals();
//EliminateLeftRecursion();          //only immidiate left recursion is eleiminated now


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


//calculating FIRST
       vector<GrammerRules>::iterator it;    //finding whether this non terminal has first calculated or not
                               for ( it=LeftRecElimGra.begin() ; it < LeftRecElimGra.end(); it++ )
                                {
                                    if(!(*it).FirstCalc)
                                    {
                                        CalculateFirst(&(*it));
                                    }

                                }

//Calculating Follow

 for ( it=LeftRecElimGra.begin() ; it < LeftRecElimGra.end(); it++ )
    {
        if((*it).Start)
        {
            (*it).Follow.push_back("$");    //applying 1st rule of follow
        }
        if(!(*it).FollowCalc)
        {
          //   cout<<"Left"<<(*it).Left<<endl;
            CalculateFollow(&(*it));

        }
    }

cout<<"FIRST PRINTING"<<endl;
//       vector<GrammerRules>::iterator it;    //finding whether this non terminal has first calculated or not
                               for ( it=LeftRecElimGra.begin() ; it < LeftRecElimGra.end(); it++ )
                                {
                                  cout<<(*it).Left<<" "<<(*it).FirstCalc<<" ";
                                  vector<string>::iterator it2;
                                  for ( it2=(*it).First.begin() ; it2 < (*it).First.end(); it2++ )
                                    {
                                        cout<<(*it2)<<" ";
                                    }
                                    cout<<endl;
                                }


     cout<<"FOLLOW PRINTING"<<endl;
//       vector<GrammerRules>::iterator it;    //finding whether this non terminal has first calculated or not
for ( it=LeftRecElimGra.begin() ; it < LeftRecElimGra.end(); it++ )
{
    cout<<(*it).Left<<" "<<(*it).FollowCalc<<" ";
    vector<string>::iterator it2;
    for ( it2=(*it).Follow.begin() ; it2 < (*it).Follow.end(); it2++ )
    {
        cout<<(*it2)<<" ";
    }
        cout<<endl;
}






vector<GrammerRules>::iterator it1;
for(it1=RulesObj.begin();it1<RulesObj.end();it1++)
{
    if((*it1).Start)
    {
        Items Item;
        Item.Left.assign((*it1).Left);
        Item.Right.assign((*it1).Right.at(0));
        Item.DotPos=0;           // DotPos 0 means its before )th positon in Right Rule
        vector<Items> Input;
        Input.push_back(Item);
        vector<Items> Output;

        Closure(Input,&Output);

        LR0Items CanItem0;
        CanItem0.StateItems=Output;
        CanItem0.State=NumberOfStates++;
        CanItems.push_back(CanItem0);





        cout<<"PRINTING CLOSURE"<<endl;
        vector<Items>::iterator it2;
        for(it2=Output.begin();it2<Output.end();it2++)
        {
            cout<<(*it2).Left<<"-->"<<(*it2).DotPos<<" "<<(*it2).Right<<endl;
        }

        /* vector<Items> Goto;
        GOTO(Output,"T",&Goto);
        cout<<"PRINTING GOTO"<<endl;
        for(it2=Goto.begin();it2<Goto.end();it2++)
        {
            cout<<(*it2).Left<<"-->"<<(*it2).DotPos<<" "<<(*it2).Right<<endl;
        }*/

        ComputeCanonicalItems();

        cout<<"PRINTING Canonical Items"<<endl;
        vector<LR0Items>::iterator it3;
        for(it3=CanItems.begin();it3<CanItems.end();it3++)
        {
            cout<<(*it3).State<<endl;

            vector<Items>::iterator it4;
            for(it4=(*it3).StateItems.begin();it4<(*it3).StateItems.end();it4++)
            {
                cout<<(*it4).Left<<"-->"<<(*it4).DotPos<<" "<<(*it4).Right<<endl;
            }

            vector<GotoInfo>::iterator it5;
            for(it5=(*it3).Goto.begin();it5<(*it3).Goto.end();it5++)
            {
                cout<<(*it5).State<<" "<<(*it5).Symbol<<endl;
            }
            cout<<"###################################################################"<<endl;
        }

        MakeTable();


        cout<<"PRINTING SLR TABLE"<<endl;
        for( map<ParsingTableKey,PTEntry,cmp_Func>::iterator ii=ParsingTable.begin(); ii!=ParsingTable.end(); ++ii)
        {
            cout << (*ii).first.Row <<" , "<<(*ii).first.Col<< " : " << (*ii).second.Left <<"-->"<<(*ii).second.Right<<" St " <<(*ii).second.State<<" Shift/Reduce "<< (*ii).second.s_r<<" Acc/Goto "<<(*ii).second.A_G<<" ACC "<<(*ii).second.Accept<<endl;
        }


         Main();       //calling Lexical analyzer, it will popolate symboltable
     //    cout<<"LEXEMEVECTOR "<<lexemeVector.size()<<endl;





        //Input Parsing
        //string Inp="~i*~i+~i$";
      //  string Inp="~f(~i<~u~|~i>~u~&~i~=~i)~i=~u;$";
      string Inp="~f(~i<~u)~i=~u;$";
       // string Inp="~i+~i*(~i-~i)+(~i-~i)*~i$";
        //string Inp;
        //ReadLexFile(&Inp);             //for reading lex tokens from file
        //ReadSymbolTable(&Inp); // Reading Tokens from Symbol table
        cout<<"Input String"<<Inp<<endl;

        ParsingStack.push("0");
        PrintParsingStack.push_back("0");
        cout<<"############################################"<<endl;
        cout<<"PRINTING PARSING STEPS"<<endl;
        InputParsing(Inp);
        cout<<"############################################"<<endl;

        break;
    }
}

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

                    //cout << "Substring: " << i <<" "<< str[i] << endl;
                    i++;
            } while (iss);

            vector<GrammerRules>::iterator it;
            bool found =false;

            if(!str[0].compare(""))
            {
               // cout<<"here";
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
                        Obj.FollowCalc=false;
                        Obj.FirstCalc=false;
                        Obj.First.clear();
                        Obj.Follow.clear();

                        if(!FindStart)   //here assuming first line in Grammar.txt file is start symbol
                        {
                            Obj.Start=true;
                            FindStart=true;
                        }else{
                            Obj.Start=false;
                        }
                        RulesObj.push_back(Obj);
                    }
}


void Closure(vector<Items> Input,vector<Items> * Output)
{
    vector<Items>::iterator it;

    for ( it=Input.begin() ; it < Input.end(); it++ )        //applying Rule 1 of the book
    {
        Output->push_back(*it);
    }
    bool StopFlag=true;

    while(1)
    {
      //  cout<<"Infi"<<endl;
        if(StopFlag==false)   //this is fro stopping condition,
        {
            break;
        }
        else    // this is because, if there is any addition in last iteration, then we need to check for new items added , so that if they satisy condition for closure
        {
            StopFlag=false;
        }

        vector<GrammerRules>::iterator it1;
        for(it1=RulesObj.begin();it1<RulesObj.end();it1++)    //checking for each non terminal
        {
            vector<Items>::iterator it2;
            int i=0;
            while(i<Output->size())
            {
                //applying Rule 1 of the book, caanot insert in vector on whcih iterator is working

                if(Output->at(i).DotPos<Output->at(i).Right.length())   //checking if . is in the end
                {
                    char ch=Output->at(i).Right.at(Output->at(i).DotPos);


                    if((*it1).Left.at(0)==ch)
                    {
                        EnterIntoClosure((*it1),Output,&StopFlag);    //checking for duplicates

                    }
                }

            i++;
            }
        }
    }
}


void EnterIntoClosure(GrammerRules Obj,vector<Items> *Output,bool *StopFlag)
{
    vector<Items>::iterator it2;
    vector<string>::iterator it3;
    for(it3=Obj.Right.begin();it3<Obj.Right.end();it3++)
    {
        bool found=false;

        for ( it2=Output->begin() ; it2 < Output->end(); it2++ )   //checking for duplicates
        {
            if( !(*it3).compare((*it2).Right) && (*it2).DotPos==0 && !Obj.Left.compare((*it2).Left) )
            {
                found=true;

            }
        }
        if(!found)
        {
            Items It;
            It.Left.assign(Obj.Left);
            It.Right.assign(*it3);
            It.DotPos=0;
            Output->push_back(It);
            *StopFlag=true;
        }
    }
}

void GOTO(vector<Items> Input,string X,vector<Items> * Output)
{
    vector<Items> ClosureInput;

    vector<Items>::iterator it;

    for ( it=Input.begin() ; it < Input.end(); it++ )
    {
        if((*it).DotPos<(*it).Right.length())
        {
            char ch=(*it).Right.at((*it).DotPos);
            string str;
            str.append(1,ch);
            if(ch=='~')          //handling the case of ~i, as its a two letter symbol, and sinlge ~ if passed two GOTO is wrror
            {
                (*it).DotPos++;
                if((*it).DotPos<(*it).Right.length())
                {
                    char ch2=(*it).Right.at((*it).DotPos);

                    str.append(1,ch2);
                }else{
                    cout<<"ERROR epsilon cannot be pased as grammer symbol into GOTO"<<endl;
                }
            }
            //if(ch==X.at(0))
            if(!str.compare(X))
            {
            Items It;
            It.Left.assign((*it).Left);
            It.Right.assign((*it).Right);
            It.DotPos=(*it).DotPos+1;
            ClosureInput.push_back(It);
            }
        }
    }
    Closure(ClosureInput,Output);
}


void ComputeCanonicalItems()
{
    //map<string,vector<Items>> Processing;
    int i=0;
    while(i<CanItems.size())           //outer loop, calculates LR0 Items
    {
      //  cout<<"Num "<<NumberOfStates<<" Size "<<CanItems.size()<<"I "<<i<<endl;
        vector<Processing> ProVector;
        ProVector.clear();

        //  LR0Items *Itemi=&CanItems.at(i);

        vector<Items>::iterator it2;
        for(it2=CanItems.at(i).StateItems.begin();it2<CanItems.at(i).StateItems.end();it2++)           // loop for segregating different Grammer Symbols after dot, so that LR0 Items can be computed
        {
            //cout<<"Items Left"<<(*it2).Left<<" Right "<<(*it2).Right<<" Dot Pos "<<(*it2).DotPos<<" I "<<i<<endl;

            Processing ProObj;

            if((*it2).DotPos<(*it2).Right.length())       //case in wich dot is in the last is left
            {
               //str.at(0)= (*it2).Right.at((*it2).DotPos);
               //str=(*it2).Right.at((*it2).DotPos);
               bool epsilon= false;
                string str;
               char ch=(*it2).Right.at((*it2).DotPos);
               str.append(1,ch);
               if(ch=='~')             //checking for the case of ~ or ~i, because ~i is two letter symbol
               {
                    if(((*it2).DotPos+1)<(*it2).Right.length())
                    {
                        char ch2=(*it2).Right.at((*it2).DotPos+1);
                        str.append(1,ch2);
                    }else{
                        epsilon=true;
                    }
                }
                if(!epsilon)      //epsilon production should not be added for calculating GOTO, to next state
                {

                    vector<Processing>::iterator it3;
                    bool found =false;

                    for(it3=ProVector.begin();it3<ProVector.end();it3++)      //mainting Items associated with each Grammer Symbol seprately, so Goto can be calculated easily
                    {
                    if(!(*it3).DotSymbol.compare(str))
                    {
                        found=true;
                        (*it3).ProItems.push_back(*it2);             // if Grammer Symbol already there, just push another item associated with it
                        break;
                    }
                    }
                    if(!found)       //entering new grammer symbol and associated item
                    {
                    Processing Obj;
                    Obj.DotSymbol.assign(str);
                    Obj.ProItems.push_back(*it2);
                    Obj.Goto.clear();
                    ProVector.push_back(Obj);
                    }
                }
            }
        }


        vector<Processing>::iterator it3;
        for(it3=ProVector.begin();it3<ProVector.end();it3++)
        {
            //cout<<"GOTO "<<(*it3).DotSymbol<<endl;
            GOTO((*it3).ProItems,(*it3).DotSymbol,&((*it3).Goto));          //computing Goto Closure

          //  PrintItems((*it3).Goto);
        }

        bool found=false;;
        for(it3=ProVector.begin();it3<ProVector.end();it3++)
        {
            found=false;
            int j=0;
            while(j<CanItems.size())              //checkig whether state previousp;y exists or not
            {

                found=vectorCompare(CanItems.at(j).StateItems,(*it3).Goto);
             //   cout<<"GOTO "<<(*it3).DotSymbol<<" I "<<i<<" Found "<<found<<"J"<<j<<endl;
               // PrintItems((*it3).Goto);

                if(found)
                {
                    //cout<<"FOUND "<<(*it3).DotSymbol<<endl;
                    GotoInfo Obj;                  //mainting Goto Info in Parent
                    Obj.State=CanItems.at(j).State;
                    Obj.Symbol=(*it3).DotSymbol;
                    CanItems.at(i).Goto.push_back(Obj);
                    break;
                }
                j++;
            }

//cout<<"AT1"<<endl;
            if(!found)
            {
             //   cout<<"Inserting Into Can Items DotSym"<<(*it3).DotSymbol<<" StateNo "<<NumberOfStates<<endl;
                LR0Items CanItem;               //creating a new canonical LR0 item
                CanItem.StateItems=(*it3).Goto;
                CanItem.State=NumberOfStates++;

                GotoInfo Obj;                  //mainting Goto Info in Parent
                Obj.State=CanItem.State;
                Obj.Symbol=(*it3).DotSymbol;
                  //cout<<"AT2"<<endl;
                CanItems.at(i).Goto.push_back(Obj);


                CanItems.push_back(CanItem);   //pushing in the array of canonical items
            }
        }
        //cout<<"END"<<CanItems.size()<<endl;
        i++;
    }
}

bool vectorCompare(vector<Items> Vect1,vector<Items> Vect2)
{
    if(Vect1.size()!=Vect2.size())      //here there will not be any duplicates in the Vectors as they are calculated by Goto and Closure
    return false;

        vector<Items>::iterator it2;
        bool found=false;
        for(it2=Vect1.begin();it2<Vect1.end();it2++)
        {
            found=false;
            vector<Items>::iterator it3;
            for(it3=Vect2.begin();it3<Vect2.end();it3++)
            {
                if( !(*it2).Left.compare((*it3).Left) && !(*it2).Right.compare((*it3).Right) && (*it2).DotPos==(*it3).DotPos)
                {
                    //cout<<(*it2).Left<<" "<<(*it3).Left<<" "<<(*it3).Right<<" "<<(*it3).Right<<" "<<(*it2).DotPos<<" "<<(*it3).DotPos<<endl;
                    found=true;
                }


            }
            if(!found)
            {
                break;
            }


        }
        if(!found)
        {
            return false;
        }else{
            return true;
        }
}

void PrintItems(vector<Items> Item)
{
     vector<Items>::iterator it3;
            for(it3=Item.begin();it3<Item.end();it3++)
            {
                 cout<<"Items Left"<<(*it3).Left<<" Right "<<(*it3).Right<<" Dot Pos "<<(*it3).DotPos<<endl;
            }
}



// Functions copied from LL1.cpp. will be removed and linked later

void CalculateFollow(GrammerRules *Obj)
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

void EnterFollow(vector<string> * Follow,vector<string> First)
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
                                    }
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

                        if(epsilon)
                        {
                            Obj->First.push_back("~");
                        }

                }
                     Obj->FirstCalc=true;

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

/*void EliminateLeftRecursion2()
{
      vector<string>::iterator it1;
      for ( it1=NonTerminals.begin() ; it1 < NonTerminals.end(); it1++ )      //considering every possible Ai
        {
            vector<GrammerRules>::iterator it;
            for ( it=LeftRecElimGra.begin() ; it < LeftRecElimGra.end(); it++ )           //Ai
            {
                if(!(*it).Left.compare((*it1)))
                {
                    vector<string>::iterator it2;
                    for ( it2=NonTerminals.begin() ; it2 < it1; it2++ )          //ordering Aj
                    {
                        vector<string>::iterator it3;
                        for ( it3=(*it).Right.begin() ; it3 < (*it).Right.end(); it3++ )       //Aj
                        {
                            string str;
                            str.append(1,(*it3).at(0));
                            if(!str.compare(*it2))
                            {
                                vector<string>::iterator it4;
                                for ( it3=(*it).Right.begin() ; it3 < (*it).Right.end(); it3++ )       //Aj
                                {

                                }
                            }
                        }

                    }
                }

            }

        }
}*/

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

void MakeTable()
{
     vector<LR0Items>::iterator it;
     for ( it=CanItems.begin() ; it < CanItems.end(); it++ )
     {
         ParsingTableKey Key;
         PTEntry Entry;
         //Key.Row=(*it).State;
         char stri[10];
         sprintf(stri,"%d",(*it).State);
         Key.Row.assign(stri);
         vector<GotoInfo>::iterator it1;
         for ( it1=(*it).Goto.begin() ; it1 < (*it).Goto.end(); it1++ )
         {
             char ch=(*it1).Symbol.at(0);
             if(ch>=65&&ch<=90)     // Goto symbol to nex state is Non terminal so it consitutes GOTO part of parsing table
             {
                 Key.Col=(*it1).Symbol;
                 Entry.A_G=false;
                 Entry.Accept=false;

                 //sprintf(Entry.State.,"%d",(*it1).State);
                 Entry.State=(*it1).State;
                 InsertParsingTable(Key,Entry);
             }
             else{
                    Key.Col=(*it1).Symbol;
                    Entry.A_G=true;
                    Entry.s_r=true;
                    Entry.Accept=false;
                    Entry.State=(*it1).State;
                    InsertParsingTable(Key,Entry);
                 }
         }




         vector<Items>::iterator it2;
         for ( it2=(*it).StateItems.begin() ; it2 < (*it).StateItems.end(); it2++ )
         {
             if( (*it2).DotPos==(*it2).Right.length() || (!(*it2).Right.compare("~")))      //dot is in the end, or epsilon transition
             {
                 if(!ifStart((*it2).Left))     //not for starting symbol
                 {
                    vector<string> Follow;
                    ReturnFollow((*it2).Left,&Follow);
                    vector<string>::iterator it3;
                    for ( it3=Follow.begin() ; it3 < Follow.end(); it3++ )
                    {
                        Key.Col=(*it3);
                        Entry.A_G=true;
                        Entry.s_r=false;
                        Entry.Accept=false;
                        Entry.Left=(*it2).Left;
                        Entry.Right=(*it2).Right;
                        InsertParsingTable(Key,Entry);
                    }
                 }else{         // s`-->S. case
                      Key.Col="$";
                      Entry.A_G=true;
                      Entry.s_r=false;
                      Entry.Accept=true;
                      InsertParsingTable(Key,Entry);
                    }
             }

         }
    }

}


void ReturnFollow(string Left,vector<string> * Output)
{
     vector<GrammerRules>::iterator it;
     for ( it=LeftRecElimGra.begin() ; it < LeftRecElimGra.end(); it++ )
     {
         if(!(*it).Left.compare(Left))
         {
             *Output=(*it).Follow;
             break;
         }
     }
}

bool InsertParsingTable(ParsingTableKey Key,PTEntry Entry)
{
    pair<map<ParsingTableKey,PTEntry,cmp_Func>::iterator,bool> ret;

    ret=ParsingTable.insert(pair<ParsingTableKey,PTEntry>(Key,Entry));
    if(!ret.second)
    {
        cout<<"Shift Reduce Conflict"<<endl;
    }
    return ret.second;
}

bool ifStart(string Left)
{
     vector<GrammerRules>::iterator it;
     for ( it=LeftRecElimGra.begin() ; it < LeftRecElimGra.end(); it++ )
     {
         if(!(*it).Left.compare(Left))
         {
             return (*it).Start;

         }
     }
}

void InputParsing(string Input)      //~i*~i+~i$
{
    int i=0;
    int k=0;
        //for(i=0;i<Input.length();)
        while(1)
        {
            PrintParsingSteps(Input.substr(i));
            // cout<<"LOOP "<<i<<" Top "<<ParsingStack.top()<<endl;
       //     for( map<ParsingTableKey,PTEntry,cmp_Func>::iterator ii=ParsingTable.begin(); ii!=ParsingTable.end(); ++ii)
         //   {
               // PTEntry Obj;
               // Obj.
                 map<ParsingTableKey,PTEntry,cmp_Func>::iterator it;
                 ParsingTableKey Key;
                 Key.Row=ParsingStack.top();
                 string str;
                 char ch=Input.at(i);
                 str.append(1,ch);
                 char ch2;
                 if(ch=='~')
                 {
                     ch2=Input.at(i+1);
                     str.append(1,ch2);
                 }

                 Key.Col=str;
                 it=ParsingTable.find(Key);

               //  cout<<"Key Row"<<Key.Row<<" Col "<<Key.Col<<endl;

                 if(it==ParsingTable.end())
                 {
                     cout<<"WRONG STRING"<<endl;
                     break;
                 }

             //    cout<<"State "<<(*it).second.State<<" A_G "<<(*it).second.A_G<<" S_R "<<(*it).second.s_r<<" Top "<<ParsingStack.top()<<" First Row"<<(*it).first.Row<<" Col "<<(*it).first.Col<<"Input"<<Input.at(i)<<endl;
               //  <<" Left "<<(*it).second.Left<<" Right "<<(*it).second.Right

                 if((*it).second.A_G)    //it is ACTION
                 {
                     if((*it).second.s_r && !(*it).second.Accept)  //it is shift
                     {
                         char stri[10];
                         sprintf(stri,"%d",(*it).second.State);
                         ParsingStack.push(stri);
                         PrintParsingStack.push_back(stri);
                         //cout<<"Shift"<<str<<endl;
                         if(str.at(0)=='~')
                         {
                             i+=2;
                         }else{
                             i++;
                         }
                     }
                     else if(!(*it).second.s_r && !(*it).second.Accept)                  //should not be accept state when shift reduce is happening
                     {
                         cout<<"Reduce Left "<<(*it).second.Left<<" Right"<<(*it).second.Right<<endl;

                         int j=0;
                         int len=calculateEffectiveLength((*it).second.Right);
                       //  cout<<"LEN"<<len<<endl;
                         for(j=0;j<len;j++)
                         {
                             ParsingStack.pop();
                             PrintParsingStack.pop_back();
                         }
                         map<ParsingTableKey,PTEntry,cmp_Func>::iterator it1;
                         ParsingTableKey Key1;

                         Key1.Row=ParsingStack.top();
                         //string str;
                         //str.append(1,Input.at(i));
                         Key1.Col=(*it).second.Left;

                         it1=ParsingTable.find(Key1);

                         if(!(*it1).second.A_G)
                         {
                             char stri[10];
                             sprintf(stri,"%d",(*it1).second.State);
                             string str=stri;
                             ParsingStack.push(str);
                             PrintParsingStack.push_back(str);

                         }else{
                             cout<<"ERROR"<<endl;
                             break;
                         }
                    }else if((*it).second.Accept)
                    {
                        cout<<"SUCCESS , String Accepted"<<endl;
                        break;
                    }
                 }
            }
}


int calculateEffectiveLength(string str)
{
    int i=0;
    int len=0;
    if(!str.compare("~"))       //for epsilon 0 should be returnned
    {
    }else{
        for(i=0;i<str.length();i++)
        {
            char ch=str.at(i);
            char ch2;
            if(ch=='~')
            {
                i++;
                ch2=str.at(i);
                len++;
            }else{
                len++;
                }
        }
    }
    return len;

}

void ReadLexFile(string * Input)
{
     ifstream LexFile ("LexFile.txt");
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
    }
    cout<<"                     ";
    cout<<Inp<<endl;

}
