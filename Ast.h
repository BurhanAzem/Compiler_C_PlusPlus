#pragma once
#include "STEntry.h"
#include "FileDescriptor.cpp"
#ifndef __AST__H
#define __AST__H
//ast.h

/* Definitions of list datatypes */
/* List of AST nodes */
typedef STEntry  symbol_table_entry;

typedef struct ast_list_cell
{
	AST* head; // pointer to  a tree
	ast_list* tail;  // this is the next pointer that you use typically in the list
} ast_list;

/* List of symbol table entries */
typedef struct ste_list_cell
{
	symbol_table_entry* head; // pointer to a symbol table entry
	ste_list* tail; //  this is the next pointer that you use typically in the list
} ste_list;

////////////////////////////////////////////////////////////
/* Type of an AST node. */
typedef enum
{
	ast_var_decl,		 /* Top-level variable declaration */
	ast_const_decl,		 /* II constant declaration */
	ast_routine_decl,	 /* II procedure or function decl. */
	ast_assign,			 /* Assignment statement */
	ast_if,				 /* If-then-else statement */
	ast_while,			 /* While statement */
	ast_for,			 /* For statement */
	ast_read,			 /* Read statement */
	ast_write,			 /* Write statement */
	ast_call,			 /* Procedure call */
	ast_block,			 /* Begin-end block */
	ast_return,			 /* Return statement */
	ast_var,			 /* Variable reference */
	ast_integer,		 /* Integer literal */
	ast_string,	
	ast_double, 
	ast_char,/* String literal */
	ast_boolean,		 /* Boolean literal */
	ast_times,			 /*  operator */
	ast_divide,			/* / operator */
	ast_plus,			 /* + operator */
	ast_minus,			 /* -operator */
	ast_eq,				/* = operator */
	ast_neq,			/* != operator */

	ast_lt, /* < operator */
	ast_le, /* <= operator */
	ast_gt, /* > operator */
	ast_ge, /* >= operator */
	ast_and, /* and operator */
	ast_or, /* or operator */
	ast_cand, /* cand operator */
	ast_cor, /* cor operator */
	ast_not, /* not operator */
	ast_uminus, /* unary -operator */
	ast_uplus,
	ast_eof, /* End of File */
	ast_float, /* float */
	ast_itof,
	ast_none/* convert integr to float */
}AST_type;

typedef enum{
	ast_times,			 /*  operator */
	ast_divide,			/* / operator */
	ast_plus,			 /* + operator */
	ast_minus,			 /* -operator */
	ast_eq,				/* = operator */
	ast_neq,			/* != operator */

	ast_lt, /* < operator */
	ast_le, /* <= operator */
	ast_gt, /* > operator */
	ast_ge, /* >= operator */
	ast_and, /* and operator */
	ast_or, /* or operator */
	ast_cand, /* cand operator */
	ast_cor, /* cor operator */
	ast_not, /* not operator */
	ast_uminus, /* unary -operator */
	ast_uplus,

}j_type;
///////////////////////////////////////////////////////////////////////
/* The actual datatype for an abstract syntax tree (AST) node. */
typedef struct ast_node
{
	AST_type      type; /* Type of the AST node */
	union /* The fields depend on a node's type */
	{
		struct {
			symbol_table_entry* name; /* Variable's symbol table entry */
		} a_var_decl;

		struct {
			symbol_table_entry* name; /* Constant's symbol table entry */
			int value; /* Evaluated value of constant */
		} a_const_decl;

		struct {
			symbol_table_entry* name; /* Routine's symbol table entry */
			ste_list* formals; /* List of formal parameters */
			AST_type result_type; /* Type of result (none_type for procedures) */
			AST* body; /* Body of routine */
		} a_routine_decl;

		struct {
			symbol_table_entry* lhs; /* Target of assignment */
			AST* rhs; /* Right side of assignment */
		} a_assign;

		struct {
			AST* predicate; /* Predicate */
			AST* conseq; /* Consequent */
			AST* altern; /* Alternative (NULL for if-then stmt) */
		} a_if;

		struct {
			AST* predicate; /* Predicate */
			AST* body; /* Body */
		} a_while;

		struct {
			symbol_table_entry* var; /* Index variable */
			AST* lower_bound; /* Lower iteration bound */
			AST* upper_bound; /* Upper iteration bound */
			AST* body; /* Body */
		}a_for;

		struct {
			symbol_table_entry* var; /* Target variable */
		} a_read;

		struct {
			symbol_table_entry* var; /* Source variable */
		} a_write;

		struct {
			symbol_table_entry* callee; /* Symbol table entry for function */
			ast_list* arg_list; /* List of actual arguments */
		} a_call;

		struct {
			ste_list* vars; /* Symbol table entries of local variables */
			ast_list* stmts; /* Statements in block */
		} a_block;

		struct {
			AST* expr; /* Return value */
		} a_return;

		struct {
			symbol_table_entry* var; /* Symbol table entry for variable */
		} a_var;

		struct {
			int value; /* Integer value */
		} a_integer;

		struct {
			float value;
		} a_float;

		struct {
			char* string; /* String value */
		} a_string;

		struct {
			int value; /* Boolean value */
		}a_boolean;

		struct {
				symbol_table_entry* arg; /* Argument, struct ast_node *arg; /* Argument */ 
			  	j_type type;
		} a_unary_op;

		struct {
			AST* larg; /* Argument 1 */
			AST* rarg; /* Argument 2 */
			AST_type type;
			j_type rel_type;
		} a_binary_op;

		struct {
			AST* arg;
		} a_itof;

	} f;  // union 
} AST; // ASt structure

/* Externally-visible functions: */
ast_list* cons_ast(AST*, ast_list*);
ste_list* cons_ste(symbol_table_entry*, ste_list*);
int eval_ast_expr(FileDescriptor*, AST*);
AST* make_ast_node();
void print_ast_node(FILE*, AST*);

#endif




