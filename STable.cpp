#include "STable.h"
#include <vector>
#include <iostream>


using namespace std;

STable::STable()
{
    for (int i = 0; i < 5; i++)
    {
        this->slots.emplace_back(new STList());
    }

    this->number_entries = 0;
    this->number_probes = 0;
    this->number_hits = 0;
    this->max_search_dist = 0;
    this->next = NULL;
}


STable::STable(bool fold_case_flag)
{
    for (int i = 0; i < 5; i++)
    {
        this->slots.emplace_back(new STList());
    }	this->fold_case = fold_case_flag;
    this->number_entries = 0;
    this->number_probes = 0;
    this->number_hits = 0;
    this->max_search_dist = 0;
    this->next = NULL;
}

unsigned long STable::ElfHash(char* str)
{
    unsigned long   h = 0, high; // h=0, g
    unsigned char* s = (unsigned char*)str;
    while (*s)
    {
        h = (h << 4) + *s++;  //1/4 of bits
        if (high = h & 0xF0000000)
            h ^= high >> 24; // h = h ^ (g >>24)
        h &= ~high; // in book this is h = h^g
    }
    return h % slots.size();
}

STEntry* STable::GetSymbol(char* str)
{
    unsigned long index = ElfHash(str);
    STEntry* ste = slots[index]->FindEntry(str);
    if (ste)
        return ste;
    else
        return NULL;
}

char* ToLowerCase(char* str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        str[i] -= 32;
    }
    return str;
}
STEntry* STable::PutSymbol(char* str, STE_TYPE type)
{
    if (this->fold_case)
        str = ToLowerCase(str);
    unsigned long index = ElfHash(str);
    STEntry* Added = slots[index]->AddEntry(str, type);
    return Added;
}






void STable::ClearSymbolTable()
{
    unsigned long i;
    for (i = 0; i < slots.size(); i++)
    {
        slots[i]->Clear();
    }
    slots.clear();
}


void STable::PrintSymbolStats()
{
    cout << "--> All Entrys\n\n";
    int c = 0;
    for (int i = 0; i < slots.size(); i++)
    {
        if (slots[i]->size() != 0) {
            cout << "Entry # " << c + 1 << ": ";
            slots[i]->PrintAll();
            c++;
        }
    }

    cout << "\n\n";
    cout << "--> Number Of Entrys: ";

    int sum = 0;

    for (int i = 0; i < slots.size(); i++)
    {
        sum += slots[i]->Count();
    }

    cout << sum << endl;

    cout << "\n";
    cout << "--> Maximum searched Entry: ";

    STEntry* maxSearchedEntry = new STEntry();

    for (int i = 0; i < slots.size(); i++)
    {
        STEntry* Entry = slots[i]->MaxSearchedEntry();
        if (Entry->SearchedTimes > maxSearchedEntry->SearchedTimes)
        {
            maxSearchedEntry = Entry;
        }
    }
    cout << maxSearchedEntry->Name << " Entry" << endl;
}


STable::~STable()
{
    ClearSymbolTable();
}




