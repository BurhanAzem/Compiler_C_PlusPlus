#pragma once
#ifndef STABLE
#define STABLE
#include "STList.h"
#include "StEntry.h"
#include <vector>




class STable
{
private:
    std::vector<STList*> slots; //Pointer to hash table array of entries
    bool fold_case; // Non-zero => fold upper to lower case 
    // Statistics on hash table effectiveness 
    int number_entries; // Number of entries in table 
    int number_probes; // Number of probes into table 
    int number_hits; // Number of probes that immediately found entry 
    int max_search_dist; // Maximum entries searched 
    STable* next; // To be used to create a stack of symbol table 
    // add your defined functions, e.g hash .
    // Externally-visible functions
public:

    STable();

    STable(bool fold_case_flag);

    unsigned long ElfHash(char* str);
    STable* next;
    STEntry* GetSymbol(char* str);


    STEntry* PutSymbol(char* str, STE_TYPE type);


    void enter_scope(); /*Inform the symbol table that the parser is entering a new scope */
    void exit_scope();  /*Inform the symbol table that the parser is leaving a scope.*/
    void ClearSymbolTable();


    void PrintSymbolStats();

    ~STable();
};

#endif // STLIST_H