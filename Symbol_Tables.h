#pragma once
#ifndef __Symbol_Tables__H
#define __Symbol_Tables__H
#define _CRT_SECURE_NO_WARNINGS
#include "STable.h"

class Symbol_Tables{
public:
	STable* head;

	Symbol_Tables();
	void enter_scope(); /*Inform the symbol table that the parser is entering a new scope */
	void exit_scope();  /*Inform the symbol table that the parser is leaving a scope.*/
};

#endif
