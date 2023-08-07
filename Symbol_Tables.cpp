#include "Symbol_Tables.h"


	Symbol_Tables::Symbol_Tables()
	{
		this->head = NULL;

	}

	void Symbol_Tables::enter_scope()
	{
		STable* stable = new STable();
		stable->next = this->head;
		this->head = stable;
	}
	void Symbol_Tables::exit_scope()
	{
		this->head = this->sTable->next;
	}
