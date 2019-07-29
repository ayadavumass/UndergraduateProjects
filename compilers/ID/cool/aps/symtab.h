#ifndef _HASH_H_
#define _HASH_H_

#include <stdio.h>
#include <malloc.h>

#include "aps-tree.h"

/* NodeInfoP can be used to carry other info, */
/* user can simply redefine this line         */
typedef enum { BASIC, PHYLUM, LIST, CONSTRUCTOR } TYPE;

typedef struct _NodeInfo {
    Symbol sym;
    TYPE type;
} NodeInfo;
typedef NodeInfo *NodeInfoP;

/* typedef int SymRec; */
typedef struct _symrec {
     NodeInfoP node;
} SRec;

typedef SRec* SymRec;

typedef struct _hashnode {
	struct _hashnode *hnext, *hprev;
	struct _hashnode *snext, *sprev;
	int key;
	SymRec sym;
} HNode;
typedef HNode* HNodeP;

typedef struct _scoperec {
	struct _scoperec* next;
	HNodeP scopeStart;
} ScopeRec;
typedef ScopeRec* ScopeRecP;

typedef struct _hashtable {
	int buckets;
	HNodeP* bucketArray;
	ScopeRecP scopeStack;
} SymTab;
typedef SymTab* SymTabP;

SymTabP makeSymTable(int size);
int symTabHash(SymTabP symTab, int key);
void symTabExitScope(SymTabP symTab);
void symTabEnterScope(SymTabP symTab);
SymRec symTabProbe(SymTabP, int key);
int symTabAddSym(SymTabP symTab, int key, SymRec symRec);
void symTabDump(FILE* fp, SymTabP symTab);
int symTabScopeCheckKey(SymTabP symTab, int key);

/* Here are some function calls we defined to manipulate the symbol table */
int Symbol_to_Key(Symbol);
int AddSym(SymTabP, Symbol, TYPE);
int inSymTab(SymTabP, Symbol);
TYPE symType(SymTabP, Symbol);

#endif
