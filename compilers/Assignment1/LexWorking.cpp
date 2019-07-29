#include<iostream>
#include<list>
#include <fstream>
#include<stdlib.h>
#define BUFFER_SIZE 4
using namespace std;

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

enum Return{
	SUCCESS,
	FAIL,
	INTER
};

enum LexicalAnalysisType{
	Keywords,
	Identifiers,
	Operators
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


/*TOKEN getNumbers()
{
	TOKEN retToken;
	retToken.Code=NUM;
	char c;
	switch(state) {
		case 12: c=nextChar();
			 if(Digits(c)) state=13;
			 else fail();
			 break;
		case 13: c=nextChar();
			 if(Digits(c)) state=13;
			 else if(c=='.') state=14;
			 else if(c=='E') state=16;
			 else if(Others(c)) state=20;
			 else fail();
			 break;
		case 14: c=nextChar();
			 if(Digits(c)) state=15;
			 else fail();
			 break;
		case 15: c=nextChar();
			 if(Digits(c)) state=15;
			 else if(c=='E') state=16;
			 else if(Others(c)) state=21;
			 else fail();
			 break;
		case 16: c=nextChar();
			 if(c=='+' || c=='-') state=17;
			 else if(Digits(c)) state=18;
			 else fail();
			 break;
		case 17: c=nextChar();
			 if(Digitd(c)) state==18;
			 else fail();
			 break;
		case 18: c=nextChar();
			 if(Digits(c)) state=18;
			 else if(Others(c)) state=19;
			 else fail();
			 break;
		case 19: retract();
			 //return(retToken,installNUM);
			 break;
		case 20: retract();
			// return (retToken,installNUM);
			 break;
		case 21: retract();
			 return (retToken,installNUM);
			 break;
	}

}

void parseWhiteSpace()
{
	switch(state) {
		case 22: c=nextChar();
			 if(Delim(c)) state=23;
			 else fail();
			 break;
		case 23: c=nextChar();
			 if(Delim(c)) state=23;
			 else if(Other(c)) state=24;
			 else fail();
			 break;
		case 24: retract();
			 break;
	}

}*/

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
			if((input>=48&&input<=57)||(input>=65&&input<=90)||(input>=97&&input<=122)||input==39)
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

void InputBuffering(DFANode * KeywordStartNode,DFANode * OperatorStartNode)
{
	char Buffer[2*BUFFER_SIZE],ch;
	int forward=2*BUFFER_SIZE,lexemeBegin=0;
	LexicalAnalysisType Type=Keywords;
	int CurrState=0;
	

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
						forward=0;

					}
					break;
				}
		}
		if(Buffer[forward]==EOF)
		{
			run=false;
		}
		Return Value;
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
		if(Value==INTER)
		{
			forward++;

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

			while(count<num)
			{
				cout<<Buffer[i];
				i++;
				if(i>=2*BUFFER_SIZE)
				{
					i=0;
				}
				count++;
				
			}
			/*for (i=lexemeBegin;i<forward;i++)
			{
				//cout<<Buffer[i];
			}*/
			cout<<endl;
			lexemeBegin=forward;
			
			CurrNode=KeywordStartNode;
			CurrState=0;
			OpCurrNode=OperatorStartNode;
			buf2=true;
			buf1=true;
			if(lexemeBegin==BUFFER_SIZE)
			{
				buf2=false;
			}
			Type=Keywords;
		}else if(Value==FAIL)
		{
			if(Type==Keywords)
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
				forward=lexemeBegin;

			}else if(Type==Operators){
				forward++;
				lexemeBegin=forward%(2*BUFFER_SIZE);
				CurrState=0;
				OpCurrNode=OperatorStartNode;


				Type=Keywords;
				buf2=true;
				buf1=true;
				CurrNode=KeywordStartNode;
			}

		}
	}
is.close();
}

int main()
{
	list<DFANode*> KeywordPrintList;
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

	InputBuffering(KeywordStartNode,OperatorStartNode);

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
	return 0;
}

