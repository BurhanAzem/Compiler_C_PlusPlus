#pragma once
#ifndef __PARSER__H
#define __PARSER__H
#define _CRT_SECURE_NO_WARNINGS
#include "Symbol_Tables.h"
#include "Scanner.h"
#include "Ast.h"



class Parser {
private:

public:
	SCANNER scanner;
	Symbol_Tables* symbol_Tables;

	AST* ParseProgram();
	AST* ParseDecl_list();
	AST* ParseDecl();
	AST* ParseExp();
	ste_list* ParseFormal_list();
	AST* ParseBlock();
	ste_list* ParseFormal_list_bar();
	void ParseVar_decl_list(ste_list*);
	ste_list* ParseFormal();
	void ParseFormal_bar(ste_list*);
	ast_list* ParseStmt_list();
	symbol_table_entry* ParseVar_decl();
	ast_list* ParseArg_list();
	ast_list* ParseArg_list_bar();
	ast_list* ParseArgs();
	void ParseArgs_bar(ast_list*);
	ast_list* Parser::ParseAdd_op();
	ast_list* Parser::ParseMul_op();
	ast_list* Parser::ParseRel_op();
	ast_list* Parser::ParseRel_conj();
	ast_list* Parser::ParseUnary_op();

};

#endif




