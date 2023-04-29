#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "codecA.h"
#include "codecB.h"

int main(int argc, char* argv[])
{
    // In order to ensure that the libraries in the project's directory are loaded first,
    // we run the following command:
    system("export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.");

    // If the user did not execute the program correctly, we throw an error message and return 1.
    if(argc != 3)
    {
        printf("Error: Wrong input. Correct usage: ./%s [Codec A/B] [Your Message]\n", argv[0]);
        printf("Please try again.\n");
        return 1;
    }

    char* codec = argv[1];
    char* msg = argv[2];

    if(strcmp(codec, "codecA") == 0) // If the user chose codecA, we decode the message using codecA's decode function.
    {
        decode_A(msg);
        printf("Decoded message: %s\n", msg);
    }
    else if(strcmp(codec, "codecB") == 0) // Likewise for codecB.
    {
        decode_B(msg);
        printf("Decoded message: %s\n", msg);
    }
    else // If the user chose an unknown codec, we print an error message and return 1.
    {
        printf("Error: Unknown codec: %s\n. Please re-execute with a valid codec.", codec);
        return 1;
    }

    return 0;
}