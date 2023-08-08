#include "Parser.h"

Parser::Parser()
{
	STable* global_stable = new STable();
	this->symbol_Tables->head = global_stable;
}


STE_TYPE getSteType(LEXEME_TYPE type)
{
	switch (type)
	{
	case lx_integer:
		return STE_INT;
	case lx_string:
		return STE_STRING;
	case lx_float:
		return STE_FLOAT;
	case lex_double:
		return STE_DOUBLE;
	case lex_char:
		return STE_CHAR;
	default :
		return STE_NONE; //0
	}
}
/*


<program> → <decl_list>              ----------- LL1() Done

--------------------------------------------------------

<decl_list> → <decl> ;' <decl_list>  ----------- LL1() 
			  | λ

--------------------------------------------------------

< decl> → var id ':' <type>
	    | constant id ' =' < expr>
		| function id <formal_list> ':' <type> <block>
		| procedure id <formal_list> <block>

--------------------------------------------------------	
		<type> → integer
		| boolean
		| string
--------------------------------------------------------
<formal_list> → '( )'
		| '(' <formals> ')'

--------------------------------------------------------


<formals> → id ':' <type>
		| <formals> ',' id ':' <type>

--------------------------------------------------------
		
<stmt> → id ' :=' <expr>
		| if <expr> then <stmt> fi
		| if <expr> then <stmt> else <stmt> fi
		| while <expr> do <stmt> od
		| for id ' :=' <expr> to <expr> do <stmt> od
		| read'('id')'
		| write' ('id ')'
		| id <arg-list>
		| return '(' <expr>' )'
		| <block>

--------------------------------------------------------

< block> → begin <var _decl_list> <stmt _list> end

--------------------------------------------------------

 <var_decl_list> → <var _decl> ';' < var _decl_list>
		| λ

--------------------------------------------------------

<var _decl> → var id ':' <type>

--------------------------------------------------------

<stmt_list> → <stmt> ';' <stmt_list>
		| λ

--------------------------------------------------------

<arg_list> → '(' ')'
		| '(' <args> ')'

--------------------------------------------------------

<args> → <expr>
		| <expr> ',' <args>

--------------------------------------------------------

<expr> → id
		| id <arg_lst>
		| integer_costant
		| string_constant
		|< expr> <arith_op> <expr>
		| true
		| false
		| <expr> <rel_op> <expr>
		|<expr> <rel_conj> <expr>
		| <unary_op> (<expr>
		| '(' <expr> ')'

--------------------------------------------------------

<arith_op> → '*' | '/' | '+' | '-'

--------------------------------------------------------

<rel_op> → ' =' | '!=' | ' <' | '<=' | ' >' | ' >='

--------------------------------------------------------

<rel_conj> → and | or

--------------------------------------------------------

< unary _op> → - | not

--------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/


/*
* 
<program> → <decl_list>

*/
Parser::ParseProgram(AST* tree)
{
	return ParseDecl_list(tree);
}


/*

<decl_list> → <decl> ;' <decl_list>  
			  | λ

*/

Parser::ParseDecl_list(AST* tree)
{
	AST* tree = ParseDecl(AST * tree);
	TOKEN* next_token = this->scanner.Scan();
	if (next_token->type != lx_semicolon)
	{
		this->scanner.Fd->reportError("Invalid grammer");
	}
	return ParseDecl_list(AST * tree);
}


/*
< decl> → var id ':' <type>
		| constant id ' =' <expr>
		| function id <formal_list> ':' <type> <block>
		| procedure id <formal_list> <block>

*/
AST* Parser::ParseDecl(AST* tree)
{
	TOKEN* next_token;
	next_token = this->scanner.Scan();
	if (next_token->type == kw_var)
	{
		next_token = this->scanner.Scan();
		char* name = next_token->str_ptr;

		if (next_token->type != lx_identifier)
		{
			this->scanner.Fd->reportError("Invalid grammer");
		}
		next_token = this->scanner.Scan();
		if (next_token->type != lx_colon)
		{
			this->scanner.Fd->reportError("Invalid grammer");
		}
		LEXEME_TYPE type = next_token->type;
		STE_TYPE ste_type = getSteType(type);
		STEntry* sTEntry = new STEntry(name, ste_type);
		this->symbol_Tables->head->PutSymbol(name, ste_type);
		return tree.make_ast_node(sTEntry, tree);
	}
	else if (next_token->type == kw_constant)
	{
		next_token = this->scanner.Scan();
		//this->symbol_Tables
		if (next_token->type != lx_identifier)
		{
			this->scanner.Fd->reportError("Invalid grammer");
		}
		next_token = this->scanner.Scan();
		if (next_token->type != lx_eq)
		{
			this->scanner.Fd->reportError("Invalid grammer");
		}
		AST* tree = ParseExp(AST * tree);
	}
	else if (next_token->type == kw_function)
	{
		next_token = this->scanner.Scan();
		//this->symbol_Tables
		if (next_token->type != lx_identifier)
		{
			this->scanner.Fd->reportError("Invalid grammer");
		}
		ParseFormal_list(AST * tree);
		next_token = this->scanner.Scan();
		if (next_token->type != lx_colon)
		{
			this->scanner.Fd->reportError("Invalid grammer");
		}
		ParseType(AST * tree);
		ParseBlock(AST * tree);
	}
	else if (next_token->type == kw_procedure)
	{
		next_token = this->scanner.Scan();
		//this->symbol_Tables
		if (next_token->type != lx_identifier)
		{
			this->scanner.Fd->reportError("Invalid grammer");
		}
		AST* tree = ParseFormal_list(AST * tree);
		AST* tree = ParseBlock(AST * tree);
	}
	return tree;
}






AST* Parser::ParseExp(AST* tree)
{

}

AST* Parser::ParseFormal_list(AST* tree)
{

}


AST* Parser::ParseBlock(AST* tree)
{

}