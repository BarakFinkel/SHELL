#include <stdio.h>
#include "codecA.h"

void general(char* msg)
{
    if(msg == NULL) return;

    char* alter_msg = msg;
    int i = 1;

    while(*alter_msg != '\0')
    {
        if(*alter_msg >= 'A' && *alter_msg <= 'Z') *alter_msg = *alter_msg + 32;
        if(*alter_msg >= 'a' && *alter_msg <= 'z') *alter_msg = *alter_msg - 32;

        alter_msg = msg[i];
        i++;
    }
}

void encode(char* msg) { general(msg); }

void decode(char* msg) { general(msg); }