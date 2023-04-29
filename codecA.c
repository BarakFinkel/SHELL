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
        else if(*alter_msg >= 'a' && *alter_msg <= 'z') *alter_msg = *alter_msg - 32;
        else;
        
        alter_msg = msg + i;
        i++;
    }
}

void encode_A(char* msg) { general(msg); }

void decode_A(char* msg) { general(msg); }