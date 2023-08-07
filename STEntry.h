#pragma once
#pragma once
#ifndef STENTRY_H
#define STENTRY_H
#include <stdio.h>
#include <string.h>
#define TYPE_SIZE 6
#include <iostream>

using namespace std;

typedef enum
{
    STE_NONE, //0
    STE_INT,
    STE_STRING,
    STE_FLOAT,
    STE_DOUBLE,
    STE_CHAR,
}STE_TYPE;

static const char* STE_TYPE_STR[TYPE_SIZE] = { "None","int","char*","float","double","char" };// It is not a good idea to put this here
static char  str[128];
class STEntry
{
public:

    char Name[64];
    STE_TYPE Type;  //it is an int
    STEntry* Next;
    int SearchedTimes;
    STEntry() {
        Next = NULL;
        Type = STE_NONE;
        Name[0] = 0; //empty String
        SearchedTimes = 0;
    }
    STEntry(char* name, STE_TYPE type)
    {
        Next = NULL;
        Type = type;
        strcpy(Name, name);
        SearchedTimes = 0;
    }
    void toString()
    {
        if ((Type < STE_NONE) || Type > STE_CHAR)  Type = STE_NONE;
        cout << "    Name: " << Name << ", Type: " << STE_TYPE_STR[Type] << "\n";
    }

    static STE_TYPE getType(char* str)
    {
        int i;
        for (i = 0; i < TYPE_SIZE; i++) {
            if (strcmp(STE_TYPE_STR[i], str) == 0) return ((STE_TYPE)i);
        }
        return STE_NONE;

    }
};

#endif // STENTRY_H
