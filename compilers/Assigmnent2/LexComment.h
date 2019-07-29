#ifndef LEXCOMMENT_H_INCLUDED
#define LEXCOMMENT_H_INCLUDED
#include<string>
#include<vector>
enum LexicalAnalysisType{
	Keywords,
	Identifiers,
	Operators,
	Number,
	WhiteSpaces,
	Punctuators,
	Comment,
	String
};
typedef struct SymbolTableStruct{
	LexicalAnalysisType Type;
	int Position;
	std::string Token;
	int lexemePos;   //stores pos of lexeme in lexemeVector array
}SymbolTableStruct;

extern std::vector<std::string> lexemeVector;
extern std::vector<SymbolTableStruct> NewSymbolTable;

int Main();


#endif // LEXCOMMENT_H_INCLUDED
