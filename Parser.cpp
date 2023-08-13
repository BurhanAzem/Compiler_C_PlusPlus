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


<program> → <decl_list>              ----------- LL1() Done         *** Done *** LL1()

--------------------------------------------------------
                                                                    *** Done *** LL1()
<decl_list> → <decl> ;' <decl_list>  ----------- LL1() 
			  | λ

--------------------------------------------------------

< decl> → var id ':' <type>                                         *** Done *** LL1()
	    | constant id ' =' < expr> 
		| function id <formal_list> ':' <type> <block>
		| procedure id <formal_list> <block>

--------------------------------------------------------	
		<type> → integer                                            *** Done *** LL1()
		| boolean
		| string 
--------------------------------------------------------
<formal_list> → '( )'
		| '(' <formals> ')'                                         *** Done *** LL1()

---> To LL1()

<formal_list> -> ( { ) | <formal> ) }
------------->

<formal_list> -> ( <formal_list_bar>    ----> LL1()
<formal_list> -> ) | <formal> )   ----> LL1()

--------------------------------------------------------


<formals> → id ':' <type>                                           *** Done ***  LL1()
		| <formals> ',' id ':' <type>

----->

<formals> -> id ';' <type> <formals_bar>               ---- LL1()
<formals_bar> -> ',' id ':' <type> <formals_bar> | λ   ---- LL1()

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

< block> → begin <var _decl_list> <stmt _list> end                *** Done *** LL1()

--------------------------------------------------------

 <var_decl_list> → <var _decl> ';' < var _decl_list>              *** Done *** LL1()
		| λ

--------------------------------------------------------

<var _decl> → var id ':' <type>                                   *** Done *** LL1()

--------------------------------------------------------

<stmt_list> → <stmt> ';' <stmt_list>                     
		| λ

--------------------------------------------------------

<arg_list> → '(' ')'                                             *** Done *** LL1()
		| '(' <args> ')'
------->

<arg_list> -> '(' <arg_list_bar>
<arg_list_bar> -> ')' | <args> ')'
--------------------------------------------------------

<args> → <expr> <args_bar>
                                                                 *** Done *** LL1()
<args_bar> → λ
		   | , <expr> <args_bar>


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

<add_op> → +
		| -                                                     *** Done ***

--------------------------------------------------------

<mul_op> → *                                                   *** Done ***
		| /

-------------------------------------------------------



<rel_op> → =
		| !=
		| <
		| <=
		| >
		| >=													*** Done ***

--------------------------------------------------------

<rel_conj> → and | or                                           *** Done ***

--------------------------------------------------------

< unary _op> → - | not                                          *** Done ***

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

		symbol_table_entry* sTEntry = new symbol_table_entry(name, ste_type);

		this->symbol_Tables->head->PutSymbol(name, ste_type);   
																
		return make_ast_node(ast_var_decl, sTEntry);  
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


		symbol_table_entry* sTEntry = new symbol_table_entry(name, STE_INT);

		this->symbol_Tables->head->PutSymbol(name, STE_INT);

		return make_ast_node(ast_const_decl, sTEntry, value);
	}
	else if (next_token->type == kw_function)
	{
		next_token = this->scanner.Scan();
		char* name = next_token->str_ptr;
		if (next_token->type != lx_identifier)
		{
			this->scanner.Fd->reportError("Invalid grammer");
		}
		ste_list* formal_list = ParseFormal_list();
		next_token = this->scanner.Scan();
		if (next_token->type != lx_colon)
		{
			this->scanner.Fd->reportError("Invalid grammer");
		}

		next_token = this->scanner.Scan();
		AST_type ast_type = getAstType(next_token->str_ptr);

		AST* blockTree = ParseBlock();

		symbol_table_entry* sTEntry = new symbol_table_entry(name, STE_STRING);

		this->symbol_Tables->head->PutSymbol(name, STE_STRING);

		return make_ast_node(ast_routine_decl, sTEntry, formal_list, ast_type, blockTree);
	}
	else if (next_token->type == kw_procedure)
	{
		next_token = this->scanner.Scan();
		char* name = next_token->str_ptr;
		if (next_token->type != lx_identifier)
		{
			this->scanner.Fd->reportError("Invalid grammer");
		}
		ste_list* formal_list = ParseFormal_list();
	
		AST* blockTree = ParseBlock();

		symbol_table_entry* sTEntry = new symbol_table_entry(name, STE_STRING);

		this->symbol_Tables->head->PutSymbol(name, STE_STRING);

		return make_ast_node(ast_routine_decl, sTEntry, formal_list, blockTree);
	}
	else
		this->scanner.Fd->reportError("Invalid grammer");
	return tree;
}




/*

<formal_list> -> ( <formal_list_bar>    ----> LL1()
<formal_list> -> ) | <formal> )         ----> LL1()

*/


ste_list* Parser::ParseFormal_list()
{

	TOKEN* next_token;
	next_token = this->scanner.Scan();
	if (next_token->type != lx_lparen)
	{
		this->scanner.Fd->reportError("Invalid grammer");
	}
	ste_list* formal_list = ParseFormal_list_bar();
	return formal_list;
}


ste_list* Parser::ParseFormal_list_bar()
{

	TOKEN* next_token;
	next_token = this->scanner.Scan();
	if (next_token->type == lx_rparen)
	{
		return;
	}
	ste_list* formal_list = ParseFormal();
	if (next_token->type != lx_rparen)
	{
		this->scanner.Fd->reportError("Invalid grammer");
	}
	return formal_list;
}

/*

<formals> -> id ';' <type> <formals_bar>               ---- LL1()
<formals_bar> -> ',' id ':' <type> <formals_bar> | λ   ---- LL1()

*/

ste_list* Parser::ParseFormal()
{
	TOKEN* next_token;
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

	if (ste_type == STE_NONE)
		this->scanner.Fd->reportError("Invalid grammer");

	symbol_table_entry* sTEntry = new symbol_table_entry(name, ste_type);

	this->symbol_Tables->head->PutSymbol(name, ste_type);
	ste_list* ste_list;

	ste_list->head = sTEntry;	
	ParseFormal_bar(ste_list);

	return ste_list;
}



void Parser::ParseFormal_bar(ste_list* ste_list)
{
	TOKEN* next_token;
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

	if (ste_type == STE_NONE)
		this->scanner.Fd->reportError("Invalid grammer");

	symbol_table_entry* sTEntry = new symbol_table_entry(name, ste_type);

	this->symbol_Tables->head->PutSymbol(name, ste_type);

	sTEntry->Next = ste_list->head;
	ste_list->head = sTEntry;

	ParseFormal_bar(ste_list);
}


/*

< block> → begin <var_decl_list> <stmt _list> end

*/

AST* Parser::ParseBlock()
{
	TOKEN* next_token;
	next_token = this->scanner.Scan();
	char* name = next_token->str_ptr;
	if (next_token->type != kw_begin)
	{
		this->scanner.Fd->reportError("Invalid grammer");
	}

	next_token = this->scanner.Scan();
	AST_type ast_type = getAstType(next_token->str_ptr);
	ste_list* var_decl_list;
	ParseVar_decl_list(var_decl_list);
	ast_list* stmt_list = ParseStmt_list();

	if (next_token->type != kw_end)
	{
		this->scanner.Fd->reportError("Invalid grammer");
	}

	symbol_table_entry* sTEntry = new symbol_table_entry(name, STE_STRING);

	this->symbol_Tables->head->PutSymbol(name, STE_STRING);

	return make_ast_node(ast_block, var_decl_list, stmt_list);
}



/*

 <var_decl_list> → <var_decl> ';' < var_decl_list>
		| λ

*/


void Parser::ParseVar_decl_list(ste_list* var_decl_list)
{
	ste_list* ste_list;
	symbol_table_entry* sTEntry = ParseVar_decl();
	if (sTEntry == NULL)
		return;
	sTEntry->Next = ste_list->head;
	ste_list->head = sTEntry;

	TOKEN* next_token;
	next_token = this->scanner.Scan();
	char* name = next_token->str_ptr;
	if (next_token->type != lx_semicolon)
	{
		return ;
	}

	ParseVar_decl_list(var_decl_list);
	return;
}


/*

<var_decl> → var id ':' <type>

*/

symbol_table_entry* Parser::ParseVar_decl()
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

		symbol_table_entry* sTEntry = new symbol_table_entry(name, ste_type);

		this->symbol_Tables->head->PutSymbol(name, ste_type);

		return sTEntry;
	}
	else
		this->scanner.Fd->reportError("Invalid grammer");
}










/*

<arg_list> -> '(' <arg_list_bar>
<arg_list_bar> -> ')' | <args> ')'


*/

ast_list* Parser::ParseArg_list_bar()
{

	TOKEN* next_token;
	next_token = this->scanner.Scan();
	if (next_token->type != lx_lparen)
	{
		this->scanner.Fd->reportError("Invalid grammer");
	}
	ast_list* formal_list = ParseArg_list_bar();
	return formal_list;
}


ast_list* Parser::ParseArg_list_bar()
{

	TOKEN* next_token;
	next_token = this->scanner.Scan();
	if (next_token->type == lx_rparen)
	{
		return;
	}
	ast_list* formal_list = ParseArgs();
	if (next_token->type != lx_rparen)
	{
		this->scanner.Fd->reportError("Invalid grammer");
	}
	return formal_list;
}

/*

<args> → <expr> <args_bar>

<args_bar> → λ
		   | , <expr> <args_bar>

*/

ast_list* Parser::ParseArgs()
{
	TOKEN* next_token;
	next_token = this->scanner.Scan();
	char* name = next_token->str_ptr;

	AST* tree = ParseExp();
	ast_list* ast_list;

	ast_list->head = tree;

	ParseArgs_bar(ast_list);

	return ast_list;
}



void Parser::ParseArgs_bar(ast_list* ast_list)

{
	TOKEN* next_token;
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

	if (ste_type == STE_NONE)
		this->scanner.Fd->reportError("Invalid grammer");

	AST* tree = ParseExp();

	this->symbol_Tables->head->PutSymbol(name, ste_type);

	tree->next = ast_list->head;
	ast_list->head = tree;

	ParseArgs_bar(ast_list);
}



/*

<add_op> → +
		| -

*/

ast_list* Parser::ParseAdd_op()
{

}


/*

<mul_op> → *                                                   
		| /


*/

ast_list* Parser::ParseMul_op()
{

}


/*

<rel_op> → =
		| !=
		| <
		| <=
		| >
		| >=

*/

ast_list* Parser::ParseRel_op()
{

}

/*

<rel_conj> → and | or

*/


ast_list* Parser::ParseRel_conj()
{

}


/*

<unary_op> → - | not

*/


ast_list* Parser::ParseUnary_op()
{

}



ast_list* Parser::ParseStmt_list()
{

}

AST* Parser::ParseExp()
{

}
