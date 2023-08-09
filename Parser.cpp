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
	case lex_boolean:
		return STE_BOOLEAN;
	default :
		return STE_NONE; //0
	}
}


AST_type getAstType(char* type)
{
	if ("int" == type)
		return ast_integer;
	if ("char*" == type)
		return ast_string;
	if ("float" == type)
		return ast_float;
	if ("bouble" == type)
		return ast_double;
	if ("char" == type)
		return ast_char;
	if ("boolean" == type)
		return ast_boolean;
	return ast_none; //0
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
AST* Parser::ParseProgram()
{
	return ParseDecl_list();
}


/*

<decl_list> → <decl> ;' <decl_list>  
			  | λ

*/

AST* Parser::ParseDecl_list()
{
	AST* tree = ParseDecl();
	TOKEN* next_token = this->scanner.Scan();
	if (next_token->type != lx_semicolon)
	{
		this->scanner.Fd->reportError("Invalid grammer");
	}
	return ParseDecl_list();
}


/*
< decl> → var id ':' <type>
		| constant id ' =' <expr>
		| function id <formal_list> ':' <type> <block>
		| procedure id <formal_list> <block>

*/
AST* Parser::ParseDecl()
{
	AST* tree = (AST*)malloc(sizeof(AST));
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

		next_token = this->scanner.Scan();
		LEXEME_TYPE type = next_token->type;
		STE_TYPE ste_type = getSteType(type);

		STEntry* sTEntry = new STEntry(name, ste_type);

		this->symbol_Tables->head->PutSymbol(name, ste_type);   // message
																// Bassam MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMESAG
		return make_ast_node((AST_type)ast_var_decl, sTEntry);  // Bassam this is call for function use variable argument (search on it) i TRID A LOT TO FIX BUT I CAN'T
	}
	else if (next_token->type == kw_constant)
	{
		next_token = this->scanner.Scan();
		char* name = next_token->str_ptr;
		if (next_token->type != lx_identifier)
		{
			this->scanner.Fd->reportError("Invalid grammer");
		}
		next_token = this->scanner.Scan();
		if (next_token->type != lx_eq)
		{
			this->scanner.Fd->reportError("Invalid grammer");
		}
		AST* ExpTree = ParseExp();

		int value = eval_ast_expr(this->scanner.Fd, ExpTree);


		STEntry* sTEntry = new STEntry(name, STE_INT);

		this->symbol_Tables->head->PutSymbol(name, STE_INT);

		return make_ast_node((AST_type)ast_const_decl, sTEntry, value);
	}
	else if (next_token->type == kw_function)
	{
		next_token = this->scanner.Scan();
		char* name = next_token->str_ptr;
		if (next_token->type != lx_identifier)
		{
			this->scanner.Fd->reportError("Invalid grammer");
		}
		AST* formal_listTree = ParseFormal_list();
		next_token = this->scanner.Scan();
		if (next_token->type != lx_colon)
		{
			this->scanner.Fd->reportError("Invalid grammer");
		}

		next_token = this->scanner.Scan();
		AST_type ast_type = getAstType(next_token->str_ptr);

		AST* blockTree = ParseBlock();

		STEntry* sTEntry = new STEntry(name, STE_STRING);

		this->symbol_Tables->head->PutSymbol(name, STE_STRING);

		return make_ast_node((AST_type)ast_routine_decl, sTEntry, formal_listTree, ast_type, blockTree);
	}
	else if (next_token->type == kw_procedure)
	{
		next_token = this->scanner.Scan();
		char* name = next_token->str_ptr;
		if (next_token->type != lx_identifier)
		{
			this->scanner.Fd->reportError("Invalid grammer");
		}
		AST* formal_listTree = ParseFormal_list();
	
		AST* blockTree = ParseBlock();

		STEntry* sTEntry = new STEntry(name, STE_STRING);

		this->symbol_Tables->head->PutSymbol(name, STE_STRING);

		return make_ast_node((AST_type)ast_routine_decl, sTEntry, formal_listTree, blockTree);
	}
	return tree;
}






AST* Parser::ParseExp()
{

}

AST* Parser::ParseFormal_list()
{

}


AST* Parser::ParseBlock()
{

}