//////////////////////////////////////////////////////////
//
// file: sample-tree.cc
//
// This file defines the functions of each class
//
//////////////////////////////////////////////////////////


#include "sample-tree.h"
#include "sample-tree.handcode.i"
#include "tree.i"


extern padder *pad;


// constructors' functions
Expression add_class::copy_Expression()
{
  return new add_class(x->copy_Expression(), y->copy_Expression());
}


void add_class::dump()
{
  pad->write();
  cout << "add";
  pad->add();
  x->dump();
  y->dump();
  pad->sub();
}


Expression iconst_class::copy_Expression()
{
  return new iconst_class(copy_String(token));
}


void iconst_class::dump()
{
  pad->write();
  cout << "iconst";
  pad->add();
  dump_String(token);
  pad->sub();
}


// interfaces used by Bison
Program nil_Program()
{
  return new nil_node<Expression>();
}

Program single_Program(Expression e)
{
  return new single_list_node<Expression>(e);
}

Program append_Program(Program p1, Program p2)
{
  return new append_node<Expression>(p1, p2);
}

Expression add(Expression x, Expression y)
{
  return new add_class(x, y);
}

Expression iconst(String token)
{
  return new iconst_class(token);
}

