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
 <var _decl_list> → <var _decl> ';' < var _decl_list>
		| λ

--------------------------------------------------------

<var _decl> → var id ':' <type>
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

*/
Parser::ParseProgram()
{
	return ParseDecl_list();
}

Parser::ParseDecl_list()
{
	ParseDecl();
}


/*
< decl> → var id ':' <type>
		| constant id ' =' < expr>
		| function id <formal_list> ':' <type> <block>
		| procedure id <formal_list> <block>

*/
Parser::ParseDecl()
{
	TOKEN* token = this->scanner.Scan();
	if (token->type == kw_var)
	{
		TOKEN* token = this->scanner.Scan();
		this->symbol_Tables
	}
}