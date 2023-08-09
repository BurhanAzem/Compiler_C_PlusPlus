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
	AST* ParseFormal_list();
	AST* ParseBlock();
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
	AST* Parse(AST*);
};

#endif




