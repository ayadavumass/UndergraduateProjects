#include<iostream>
#include<list>
#include <fstream>
#include<stdlib.h>
#include<string>
#define BUFFER_SIZE 5
#define NUM_PUNC 8
using namespace std;
char Punc[NUM_PUNC]={'(',')',',',';','{','}','[',']'};

enum LexicalAnalysisType{
	Keywords,
	Identifiers,
	Operators,
	Number,
	WhiteSpaces,
	Punctuators
};

typedef struct EdgeInfo{
	char input;
	void *DFANodePtr;
	bool other;
}EdgeInfo;

typedef struct DFANode{
	list<EdgeInfo*> Transitions;
	bool final;
	int stateid;
}DFANode;

typedef struct SymbolTableEntry{
	LexicalAnalysisType Type;
	string lexeme;
	list<int> PositionList;
}SymbolTableEntry;

enum Return{
	SUCCESS,
	FAIL,
	INTER
};




void MakeKeywordsDFA(DFANode * StartNode,list<DFANode*> * PrintList,char *Filename)
{
	int stateid=1;
	ifstream is;
	char c;
	//is.open ("keywords");
	is.open(Filename);
	DFANode * CurrNode;
	CurrNode=StartNode;  
	list<EdgeInfo*>::iterator it;

	while (is.good())     // loop while extraction from file is possible
	{
		c = is.get();       // get character from file

		if (is.good())
		{
			if(c=='\n')
			{
				DFANode * NewNode;
				NewNode =new DFANode;
				NewNode->final=true;
				NewNode->stateid=stateid++;
				PrintList->push_back(NewNode);
				EdgeInfo *NewEdge=new EdgeInfo;
				NewEdge->input='~';
				NewEdge->DFANodePtr=NewNode;
				NewEdge->other=true;
				CurrNode->Transitions.push_back(NewEdge);
				//CurrNode=NewNode;
				CurrNode=StartNode;
				
			}
			else
			{
				bool found=false;
				if((CurrNode->Transitions).size()!=0)
				{
					for (it=CurrNode->Transitions.begin();it!=CurrNode->Transitions.end();it++)
					{
						if((*it)->input==c)
						{
							found =true;
							CurrNode=(DFANode*)((*it)->DFANodePtr);
							break;
						}

					}
					if(found==false)
					{
						DFANode * NewNode;
						NewNode =new DFANode;
						NewNode->final=false;
						NewNode->stateid=stateid++;
						PrintList->push_back(NewNode);
						EdgeInfo *NewEdge=new EdgeInfo;
						NewEdge->input=c;
						NewEdge->DFANodePtr=NewNode;
						NewEdge->other=false;
						CurrNode->Transitions.push_back(NewEdge);
						CurrNode=NewNode;

					}
				}else{
					DFANode * NewNode;
					NewNode =new DFANode;
					NewNode->final=false;
					NewNode->stateid=stateid++;
					PrintList->push_back(NewNode);
					EdgeInfo *NewEdge=new EdgeInfo;
					NewEdge->input=c;
					NewEdge->DFANodePtr=NewNode;
					NewEdge->other=false;
					CurrNode->Transitions.push_back(NewEdge);
					CurrNode=NewNode;
				}

			}
			cout << c;
		}
	}

	is.close();  
}
void PrintDFA(list<DFANode*> * PrintDFA)
{
	list<DFANode*>::iterator it;
	list<EdgeInfo*>::iterator ite;
	for (it=PrintDFA->begin();it!=PrintDFA->end();it++)
	{
		for(ite=(*it)->Transitions.begin();ite!=(*it)->Transitions.end();ite++)
		{
			cout<<" From State "<<(*it)->stateid<<" On Letter "<<(*ite)->input<<" To State "<<((DFANode*)((*ite)->DFANodePtr))->stateid<<endl;

		}
	}
}
/*void MakeIndentifierDFA(DFANode * StartNode)
{
	DFANode *CurrNode=StartNode;
	int stateid=1;
	DFANode * NewNode;
	NewNode =new DFANode;
	NewNode->final=false;
	NewNode->stateid=stateid++;
	//PrintList->push_back(NewNode);
	EdgeInfo *NewEdge=new EdgeInfo;
	NewEdge->input='~';
	NewEdge->DFANodePtr=NewNode;
	NewEdge->other=false;
	NewEdge->digit=false;
	NewEdge->delimit=false;
	NewEdge->letter=true;
	CurrNode->Transitions.push_back(NewEdge);
	CurrNode=NewNode;




}*/

Return IdentifierAnalysis(char input,int * CurrState)
{
	switch(*CurrState) {
			case 0: if((input>=65&&input<=90)||(input>=97&&input<=122)) 
				{
					*CurrState=1;
					return INTER;
				}
				else 
				{
					return FAIL;
				}
				break;
			case 1: 
				 if((input>=65&&input<=90)||(input>=97&&input<=122) || (input>=48&&input<=57)) 
				 {
					 *CurrState=1;
					 return INTER;
				 }
				 else{
					 *CurrState=2;
					 return SUCCESS;
				 }
				 break;
			case 2: cout<<"Should Not Reach Here"<<endl;//retract();
				// return(getToken(),installID());
				 break;
		}
}


Return NumberAnalysis(char input,int * CurrState)
{
	switch(*CurrState) {
		case 12: if(input>=48&&input<=57) {
				 *CurrState=13;
				 return INTER;
			 }
			 else {
				 return FAIL;
			 }
			 break;
		case 13: if(input>=48&&input<=57) {
				 *CurrState=13;
				 return INTER;
			 }
			 else if(input=='.') {
				 *CurrState=14;
				 return INTER;
			 }
			 else if(input=='E') {
				 *CurrState=16;
				 return INTER;
			 }
			 else if(input==';'||input==' ') {
				 *CurrState=20;
				 return SUCCESS;
			 }
			 else {
				 return FAIL;
			 }
			 break;
		case 14: if(input>=48&&input<=57) {
				 *CurrState=15;
				 return INTER;
			 }
			 else {
				 return FAIL;
			 }
			 break;
		case 15: if(input>=48&&input<=57) {
				 *CurrState=15;
				 return INTER;
			 }
			 else if(input=='E') {
				 *CurrState=16;
				 return INTER;
			 }
			 else if(input==';'||input==' ') {
				 *CurrState=21;
				 return SUCCESS;
			 }
			 else {
				 return FAIL;
			 }
			 break;
		case 16: if(input=='+' || input=='-') {
				 *CurrState=17;
				 return INTER;
			 }
			 else if(input>=48&&input<=57) {
				 *CurrState=18;
				 return INTER;
			 }
			 else {
				 return FAIL;
			 }
			 break;
		case 17: if(input>=48&&input<=57) {
				 *CurrState=18;
				 return INTER;
			 }
			 else {
				 return FAIL;
			 }
			 break;
		case 18: if(input>=48&&input<=57) {
				 *CurrState=18;
				 return INTER;
			 }
			 else if(input==';'||input==' ') {
				 *CurrState=19;
				 return SUCCESS;
			 }
			 else {
				 return FAIL;
			 }
			 break;
		case 19: cout<<"Not Reach"<<endl;
			 return SUCCESS;
			 //return(retToken,installNUM);
			 break;
		case 20: cout<<"Not Reach"<<endl;
			 return SUCCESS;
			// return (retToken,installNUM);
			 break;
		case 21: cout<<"Not Reach"<<endl;
			 return SUCCESS;
			 //return (retToken,installNUM);
			 break;
	}

}

Return WhiteSpaceAnalysis(char input,int * CurrState)
{
	switch(*CurrState) {
		case 0: 
			 if(input=='\n'||input=='\t'||input==' ') {
				 *CurrState=1;
				 return INTER;
			 }
			 else {
				 return FAIL;
			 }
			 break;
		case 1: 
			 if(input=='\n'||input=='\t'||input==' ') {
				 *CurrState=1;
				 return INTER;
			 }
			 //else if(Other(c)) state=2;
			 else {
				 *CurrState=2;
				 return SUCCESS;
				 //fail();
			 }
			 break;
		case 2: //retract();
			 {
				 cout<<"Not Reach"<<endl;
				 return SUCCESS;
			 }
			 break;
	}

}

Return KeywordAnalysis(DFANode **CurrNode,char input)
{
	list<EdgeInfo*>::iterator it;
	bool found=false;
	for (it=(*CurrNode)->Transitions.begin();it!=(*CurrNode)->Transitions.end();it++)
	{
		if((*it)->other==true)
		{
			if((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122))
			{
				return FAIL;
			}else
			{
				return SUCCESS;
			}
			
		}else{
			if((*it)->input==input)
			{
				found =true;
				*CurrNode=(DFANode*)((*it)->DFANodePtr);
				return INTER;
				break;
			}
		}
	}
	if(found==false)
		return FAIL;
}

Return OperatorAnalysis(DFANode **CurrNode,char input)
{
	list<EdgeInfo*>::iterator it;
	bool found=false;
	for (it=(*CurrNode)->Transitions.begin();it!=(*CurrNode)->Transitions.end();it++)
	{
		if((*it)->other==true)
		{
			if((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122)||input==39||input==' ')
			{
				return SUCCESS;
			}else
			{
				return FAIL;
			}
			
		}else{
			if((*it)->input==input)
			{
				found =true;
				*CurrNode=(DFANode*)((*it)->DFANodePtr);
				return INTER;
				break;
			}
		}
	}
	if(found==false)
		return FAIL;
}

Return PunctuatorAnalysis(char input)
{
	int i=0;
	for(i=0;i<NUM_PUNC;i++)
	{
		if(input==Punc[i])
		{
			return SUCCESS;
		}
	}
	return FAIL;
}

void SymbolTableOperation(list<SymbolTableEntry> *SymbolTable,LexicalAnalysisType Type,string lexeme,int pos)
{
	list<SymbolTableEntry>::iterator it;
	bool found=false;
	for (it=SymbolTable->begin();it!=SymbolTable->end();it++)
	{
		if(lexeme.compare((*it).lexeme)==0)
		{
			found=true;
			((*it).PositionList).push_back(pos);
			break;
		}
	}
	if(found==false)
	{
		SymbolTableEntry Obj;
		Obj.Type=Type;
		Obj.lexeme=lexeme;
		Obj.PositionList.push_back(pos);
		SymbolTable->push_back(Obj);
	}
}

void InputBuffering(DFANode * KeywordStartNode,DFANode * OperatorStartNode,list<SymbolTableEntry> * SymbolTable)
{
	int pos=0;
	char Buffer[2*BUFFER_SIZE],ch;
	int forward=2*BUFFER_SIZE,lexemeBegin=0;
	LexicalAnalysisType Type=WhiteSpaces;
	int CurrState=0;
	int NumberCurrState=12;
	int WSCurrState=0;
	ifstream is;
	is.open ("inputfile",ios::binary);
	//ch= is.get();
	//cout<<"Print "<<ch;
	bool run=true;
	bool buf2=true,buf1=true;
	DFANode * CurrNode=KeywordStartNode;
	DFANode * OpCurrNode=OperatorStartNode;
	while(run)
	{
		//cout<<forward<<endl;
		switch(forward)
		{
			case BUFFER_SIZE:
				{
					if(buf2)
					{
						is.read(&Buffer[BUFFER_SIZE],BUFFER_SIZE);
						if(is.eof())
						{
							Buffer[BUFFER_SIZE+is.gcount()]=EOF;
						}
					}
					//cout<<endl<<Buffer<<endl<<endl;
					break;
				}
			case 2*BUFFER_SIZE:
				{
					if(buf1)
					{
						is.read(Buffer,BUFFER_SIZE);
						if(is.eof())
						{
							Buffer[is.gcount()]=EOF;
						}
						//cout<<endl<<Buffer<<endl<<endl;
					}
						forward=0;
						break;
				}
		}
		if(Buffer[forward]==EOF)
		{
			run=false;
		}
		Return Value;
		if(Type==WhiteSpaces)
		{
			Value=WhiteSpaceAnalysis(Buffer[forward],&WSCurrState);
		}
		if(Type==Keywords)
		{
			Value=KeywordAnalysis(&CurrNode,Buffer[forward]);
		}
		else if(Type==Identifiers)
		{
			Value=IdentifierAnalysis(Buffer[forward],&CurrState);

		}
		else if(Type==Operators)
		{
			Value=OperatorAnalysis(&OpCurrNode,Buffer[forward]);
		}
		else if(Type==Number)
		{
			Value=NumberAnalysis(Buffer[forward],&NumberCurrState);
		}
		else if(Type==Punctuators)
		{
			//cout<<"Punc"<<Buffer[forward]<<endl;
			Value=PunctuatorAnalysis(Buffer[forward]);

		}
		if(Value==INTER)
		{
			forward++;
			pos++;

		}else if(Value==SUCCESS)
		{
			int i;
			int num=forward-lexemeBegin;
			if(num<0)
			{
				num=2*BUFFER_SIZE+num;
			}
			
			i=lexemeBegin;
			int count=0;
			string lexeme="";
			if(Type!=Punctuators)
			{
				while(count<num)
				{
					//cout<<Buffer[i];
					lexeme.push_back(Buffer[i]);
					i++;
					if(i>=2*BUFFER_SIZE)
					{
						i=0;
					}
					count++;

				}
			}
			else{
				lexeme.push_back(Buffer[i]);
			}
			if(Type!=WhiteSpaces)
			{
				cout<<lexeme<<" "<<pos-num<<" "<<num;
				SymbolTableOperation(SymbolTable,Type,lexeme,pos-num);
			}
			/*for (i=lexemeBegin;i<forward;i++)
			{
				//cout<<Buffer[i];
			}*/

			
			cout<<endl;


			buf2=true;
			buf1=true;
			if(forward==BUFFER_SIZE)        // Buffer doesn't have to be reloaded again, if its on verge
			{
				buf2=false;
			}
			if(Type==Punctuators)    //in case of Punctors it doesn't see two character, so it doesn't advance automatically, Buffer can be reloaded if its at verge.
			{
				forward++;
				pos++;
			}

			//lexemeBegin=forward;
			lexemeBegin=forward%(2*BUFFER_SIZE);
			CurrNode=KeywordStartNode;
			CurrState=0;
			WSCurrState=0;
			OpCurrNode=OperatorStartNode;
			NumberCurrState=12;
			Type=WhiteSpaces;
		}else if(Value==FAIL)
		{
			if(Type==WhiteSpaces)
			{
				WSCurrState=0;
				Type=Keywords;
				if(forward>=BUFFER_SIZE&&lexemeBegin<=BUFFER_SIZE)
				{
					buf2=false;
				}
				if(lexemeBegin>BUFFER_SIZE&&forward<BUFFER_SIZE)
				{
					buf1=false;
				}
				if(forward>=lexemeBegin)
				{
					pos=pos-(forward-lexemeBegin);
				}
				else{
					pos=pos-(2*BUFFER_SIZE-lexemeBegin+forward);
				}
				
				forward=lexemeBegin;
				
			}
			else if(Type==Keywords)
			{
				CurrNode=KeywordStartNode;
				Type=Identifiers;
				if(forward>=BUFFER_SIZE&&lexemeBegin<=BUFFER_SIZE)
				{
					buf2=false;
				}
				if(lexemeBegin>BUFFER_SIZE&&forward<BUFFER_SIZE)
				{
					buf1=false;
				}

				if(forward>=lexemeBegin)
				{
					pos=pos-(forward-lexemeBegin);
				}
				else{
					pos=pos-(2*BUFFER_SIZE-lexemeBegin+forward);
				}
				
				forward=lexemeBegin;
			}else if(Type==Identifiers){
				Type=Operators;
				CurrState=0;
				if(forward>=BUFFER_SIZE&&lexemeBegin<=BUFFER_SIZE)
				{
					buf2=false;
				}
				if(lexemeBegin>BUFFER_SIZE&&forward<BUFFER_SIZE)
				{
					buf1=false;
				}

				if(forward>=lexemeBegin)
				{
					pos=pos-(forward-lexemeBegin);
				}
				else{
					pos=pos-(2*BUFFER_SIZE-lexemeBegin+forward);
				}
				forward=lexemeBegin;

			}else if(Type==Operators)
			{

				Type=Number;
				OpCurrNode=OperatorStartNode;
				if(forward>=BUFFER_SIZE&&lexemeBegin<=BUFFER_SIZE)
				{
					buf2=false;
				}
				if(lexemeBegin>BUFFER_SIZE&&forward<BUFFER_SIZE)
				{
					buf1=false;
				}


				if(forward>=lexemeBegin)
				{
					pos=pos-(forward-lexemeBegin);
				}
				else{
					pos=pos-(2*BUFFER_SIZE-lexemeBegin+forward);
				}
				forward=lexemeBegin;
			}
			else if (Type==Number){
				Type=Punctuators;
				NumberCurrState=12;
				if(forward>=BUFFER_SIZE&&lexemeBegin<=BUFFER_SIZE)
				{
					buf2=false;
				}
				if(lexemeBegin>BUFFER_SIZE&&forward<BUFFER_SIZE)
				{
					buf1=false;
				}


				if(forward>=lexemeBegin)
				{
					pos=pos-(forward-lexemeBegin);
				}
				else{
					pos=pos-(2*BUFFER_SIZE-lexemeBegin+forward);
				}
				forward=lexemeBegin;
			}
			else if(Type==Punctuators){
				cout<<"Error At position "<<pos<<" Erroreous Characters "<<Buffer[forward]<<endl;
				forward++;
				pos++;
				NumberCurrState=12;
				lexemeBegin=forward%(2*BUFFER_SIZE);
				CurrState=0;
				OpCurrNode=OperatorStartNode;
				Type=WhiteSpaces;
				buf2=true;
				buf1=true;
				CurrNode=KeywordStartNode;
			}

		}
	}
is.close();
}

void PrintSymbolTable(list<SymbolTableEntry> *SymbolTable)
{
	list<SymbolTableEntry>::iterator it;
	for (it=SymbolTable->begin();it!=SymbolTable->end();it++)
	{
		cout<<(*it).Type<<" "<<(*it).lexeme<<" ";
		list<int>::iterator itn;
		for (itn=(*it).PositionList.begin();itn!=(*it).PositionList.end();itn++)
		{
			cout<<(*itn)<<" ";
			
		}
		cout<<endl;
	}

}

int main()
{
	list<DFANode*> KeywordPrintList;
	list<SymbolTableEntry> SymbolTable;
	DFANode * KeywordStartNode ;
	KeywordStartNode=new DFANode;
	KeywordStartNode->final=false;
	KeywordStartNode->stateid=0;
	KeywordPrintList.push_back(KeywordStartNode);
	MakeKeywordsDFA(KeywordStartNode,&KeywordPrintList,"keywords");
	cout<<KeywordStartNode->Transitions.size()<<endl;
	PrintDFA(&KeywordPrintList);


	list<DFANode*> OperatorPrintList;
	DFANode * OperatorStartNode ;
	OperatorStartNode=new DFANode;
	OperatorStartNode->final=false;
	OperatorStartNode->stateid=0;
	OperatorPrintList.push_back(OperatorStartNode);
	MakeKeywordsDFA(OperatorStartNode,&OperatorPrintList,"operators");
	cout<<OperatorStartNode->Transitions.size()<<endl;
	PrintDFA(&OperatorPrintList);

	InputBuffering(KeywordStartNode,OperatorStartNode,&SymbolTable);

	//list<DFANode*> KeywordPrintList;
/*	DFANode * IdentifierStartNode ;
	IdentifierStartNode=new DFANode;
	IdentifierStartNode->final=false;
	IdentifierStartNode->stateid=0;
	//KeywordPrintList.push_back(KeywordStartNode);
	MakeIdentifierDFA(IdentifierStartNode);
	//cout<<KeywordStartNode->Transitions.size()<<endl;
	//PrintDFA(&KeywordPrintList);
*/
	PrintSymbolTable(&SymbolTable);
	return 0;
}

