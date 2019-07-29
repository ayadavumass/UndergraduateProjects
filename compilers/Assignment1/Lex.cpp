#include<iostream>
#include<list>
using namespace std;
#define MAXTKNLEN 10

int state;
char Keywords[5][10]={"int","if","else","return","char"};

/*#define MAX_OUTLINKS 10
enum Type {
	Start,
	Intermidiate,
	Final
};
struct State{
	enum Type State_Type;
	struct State * Transitions[MAX_OUTLINKS];
	char Transition_Input[MAX_OUTLINKS];
};*/


enum RelOpCode{
	LE,
	NE,
	LT,
	EQ,
	GE,
	GT
};

enum TokenTypes{
	RELOP,
	ID,
	NUM,
	KEYW
};
typedef struct TOKEN{
	enum RelOpCode Code;
	enum TokenTypes Type;
}TOKEN;

typedef struct TokenPos{
	int Pos;
	TokenPos * Ptr;
}TokenPos;

typedef struct SymbolTableList{
	char Token[MAXTKNLEN];
	TokenTypes Type;
	TokenPos * Ptr;
}SymbolTableList;

TOKEN getRelop()
{
	TOKEN retToken;
	retToken.Code=RELOP;
	char c;
	while(1)
	{
		switch(state) {
			case 0: c=nextChar();
				if(c=='<') state=1;
				else if(c=='=') state=5;
				else if(c=='>') state=6;
				else fail();
				break;
			case 1:
				c=nextChar();
				if(c=='=') state=2;
				else if(c=='>') state=3;
				if(Other(c)) state=4;
				else fail();
				break;
			case 2:retToken.Type=LE;
			       return(retToken);
			       break;
			case 3:retToken.Type=NE;
			       return(retToken);
				break;
			case 4:retract();
				retToken.Type=LT;
				return(retToken);
				break;
			case 5:retToken.Type=EQ;
			       return(retToken);
			       break;
			case 6:c=nextChar();
			       if(c=='=') state=7;
			       else if(Other(c)) state=8;
			       else fail();
			       break;
			case 7:retType.Type=GE;
			       return(retType);
				break;
			case 8:retract();
			       retToken.Type=GT;
			       return(retToken);
				break;
		}
	}
}

TOKEN getIdentifier()
{
	TOKEN retToken;
	retToken.Code=ID;
	char c;
	while(1)
	{
		switch(state) {
			case 9: c=nextChar();
				if(Letters(c)) state=10;
				else fail();
				break;
			case 10: c=nextChar();
				 if(Letters(c) || Digits(c)) state=10;
				 else if(Others(c)) state=11;
				 else fail();
				 break;
			case 11: retract();
				// return(getToken(),installID());
				 break;
		}
	}
}
TOKEN getNumbers()
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

}
int main()
{
	state=0;





}
