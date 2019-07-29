#ifndef SAMPLE_TREE_H
#define SAMPLE_TREE_H
//////////////////////////////////////////////////////////
//
// file: sample-tree.h
//
// This file defines classes for each phylum and constructor
//
//////////////////////////////////////////////////////////


#include "tree.h"
#include "sample-tree.handcode.h"


// define the class for phylum
typedef class Expression_class *Expression;

class Expression_class : public tree_node {
public:
  tree_node *copy()		 { return copy_Expression(); }
  virtual Expression copy_Expression() = 0;

#ifdef Expression_EXTRAS
  Expression_EXTRAS
#endif
};


// define the class for phylum - LIST
typedef list_node<Expression> Program_class;
typedef Program_class *Program;


// define the class for constructors
class add_class : public Expression_class {
private:
  Expression x;
  Expression y;
public:
  add_class(Expression a1, Expression a2) {
    x = a1;
    y = a2;
  }
  Expression copy_Expression();
  void dump();

#ifdef add_EXTRAS
  add_EXTRAS
#endif
};


class iconst_class : public Expression_class {
private:
  String token;
public:
  iconst_class(String a1) {
    token = a1;
  }
  Expression copy_Expression();
  void dump();

#ifdef iconst_EXTRAS
  iconst_EXTRAS
#endif
};


// define the prototypes of the interface
Program nil_Program();
Program single_Program(Expression);
Program append_Program(Program, Program);
Expression add(Expression, Expression);
Expression iconst(String);


#endif
