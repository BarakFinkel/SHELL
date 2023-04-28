#include <stdio.h>
#include "codecB.h"

void encode(char* msg)
{
    if(msg == NULL) return;

    char* alter_msg = msg;
    int i = 1;

    while(*alter_msg != '\0') 
    {
        *alter_msg = (*alter_msg + 3) % 256;
        alter_msg = msg[i];
        i++;
    }
}

void decode(char* msg)
{
    if(msg == NULL) return;

    char* alter_msg = msg;
    int i = 1;

    while(*alter_msg != '\0') 
    {
        *alter_msg = (*alter_msg - 3) % 256;
        
        alter_msg = msg[i];
        i++;
    }
}