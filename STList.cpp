#include "stlist.h"
/**
 * @brief STList::STList
 * default constructor, initialize head and Counter
 */
STList::STList()
{
    Counter = 0;
    Head = NULL;
}
/**
 * @brief STList::FindEntry: search (linear search) the list and compare the name to the ones in the list
 * @param name : Given name to find
 * @return : If name is found found return NULL otherwise reaturn a pointer to the Node
 */
STEntry* STList::FindEntry(char* name)
{
    STEntry* ste = Head;
    while (ste != NULL)
    {
        if (strcmp(name, ste->Name) == 0)
        {
            ste->SearchedTimes++;
            return ste;
        }
        ste = ste->Next;
    }
    return NULL;

}
/**
 * @brief STList::AddEntry : Call FindEntry, if name is alread in table return false, otherwise add it to the list
 *                           Add it as the first Entry, like a stack which is fastest. Update Counter and Head
 * @param name : Name of Entry (variable)
 * @param type : Type of variable
 * @return : True if the node is added and False if the Entry Already exists in the Table
 */
STEntry* STList::AddEntry(char* name, STE_TYPE type)
{
    STEntry* ste = FindEntry(name);
    if (ste)
    {
        return ste;
    }
    else
    {
        ste = new STEntry(name, type);
        ste->Next = Head;
        Head = ste;
        Counter++;
    }
    return ste;
}
/**
 * @brief STList::PrintAll : Prints All nodes in the list, use the print in the STEntry.
 * @param fp : File Pointer where to write
 */

int STList::size()
{
    int count = 0;
    STEntry* ste = Head;
    while (ste != NULL)
    {
        count++;
        ste = ste->Next;
    }
    return count;
}

void STList::PrintAll()
{
    STEntry* ste = Head;
    while (ste != NULL)
    {
        ste->toString();
        ste = ste->Next;
    }
    printf("\n");

}
/**
 * @brief STList::Count returns Counter which is Number of Elements
 * @return
 */
int STList::Count()
{
    return Counter;
}
/**
 * @brief STList::Clear : deletes all Elements in the List and sets the counter to zero
 */
void STList::Clear()
{
    STEntry* ste;
    while (Head != NULL)
    {
        ste = Head;
        Head = Head->Next;
        delete ste;
    }
    Counter = 0;
}
STEntry* STList::MaxSearchedEntry()
{
    STEntry* maxSearchedEntry = new STEntry();
    STEntry* ste = Head;
    while (ste != NULL)
    {
        if (ste->SearchedTimes > maxSearchedEntry->SearchedTimes)
            maxSearchedEntry = ste;
        ste = ste->Next;
    }
    return maxSearchedEntry;
}
STList::~STList()
{
    Clear();
}

