#include "Parser.h"

Parser::Parser()
{

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
Parser::ParseProgram()
{
	return ParseDecl_list();
}


/*

<decl_list> → <decl> ;' <decl_list>  
			  | λ

*/

Parser::ParseDecl_list()
{
	ParseDecl();
	TOKEN* next_token = this->scanner.Scan();
	if (next_token->type != lx_semicolon)
	{
		this->scanner.Fd->reportError("Invalid grammer");
	}
	ParseDecl_list();
}


/*
< decl> → var id ':' <type>
		| constant id ' =' <expr>
		| function id <formal_list> ':' <type> <block>
		| procedure id <formal_list> <block>

*/
Parser::ParseDecl()
{
	TOKEN* next_token;
	next_token = this->scanner.Scan();
	if (next_token->type == kw_var)
	{
		next_token = this->scanner.Scan();
		//this->symbol_Tables
		if (next_token->type != lx_identifier)
		{
			this->scanner.Fd->reportError("Invalid grammer");
		}
		next_token = this->scanner.Scan();
		if (next_token->type != lx_colon)
		{
			this->scanner.Fd->reportError("Invalid grammer");
		}
		ParseType();
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
		ParseExp();
	}
	else if (next_token->type == kw_function)
	{
		next_token = this->scanner.Scan();
		//this->symbol_Tables
		if (next_token->type != lx_identifier)
		{
			this->scanner.Fd->reportError("Invalid grammer");
		}
		ParseFormal_list();
		next_token = this->scanner.Scan();
		if (next_token->type != lx_colon)
		{
			this->scanner.Fd->reportError("Invalid grammer");
		}
		ParseType();
		ParseBlock();
	}
	else if (next_token->type == kw_procedure)
	{
		next_token = this->scanner.Scan();
		//this->symbol_Tables
		if (next_token->type != lx_identifier)
		{
			this->scanner.Fd->reportError("Invalid grammer");
		}
		ParseFormal_list();
		ParseBlock();
	}
}






Parser::ParseExp()
{

}

Parser::ParseFormal_list()
{

}


Parser::ParseBlock()
{

}