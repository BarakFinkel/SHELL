#include <stdio.h>
#include <string.h>

#define BUFF_SIZE 1024

int verbose = 0;

int main(int argc, char *argv[])
{
    // We check if the number of arguments is correct.
    // If not, we print an error message and exit the program.
    if (argc < 3)
    {
        printf("Error: received less variables than expected. \n");
        printf("Please execute the file as following: ./cmp <file1> <file2> (-v) (-i)\n");
        return 1;
    }

    // We go over the flags and check if they are valid.
    // If they are, we set the corresponding variable to 1.
    // If not, we print an error message, and return 1.
    for(int i=3 ; i < argc ; i++)
    {
        if (!strcmp(argv[i], "-v")) { ( verbose = 1 ); }
        else 
        { 
            if(verbose) { printf("Error: General failure. Exiting Program.\n"); } 
            return 1; 
        }
    }

    // We set file pointers to the files given by the user.
    FILE *src_file = fopen(argv[1], "r");
    FILE *target_file = fopen(argv[2], "r");

    // If the src file could not be opened propperly, 
    // we print an error message and exit the program.
    if (src_file == NULL ) 
    { 
        if(verbose)             { printf("Error: General failure. Exiting Program.\n"); } 
        if(target_file != NULL) { fclose(target_file); }
        return 1; 
    }

    // If the target_file is not NULL, we know that it already exists.
    // Therefore, we (
    if(target_file != NULL)
    {
        if(verbose) { printf("Error: Target file exists. Exiting program.\n"); }
        fclose(src_file);
        fclose(target_file);
        return 1;
    }

    // We know create a new file pointer to our target, this time for writing purposes.
    FILE *new_file = fopen(argv[2], "w");
    if(new_file == NULL)
    {
        if(verbose) { printf("Error: General failure. Exiting Program.\n"); }
        fclose(src_file);
        return 1;
    }

    // We initialize the bytes_read variable to keep track of bytes read from src_file, and the buffer array to store the bytes.
    int bytes_read;
    char buffer[BUFF_SIZE];

    // We now begin a while loop which will stop as soon as the files are done reading and are the same or if they are not the same.
    while (1)
    {
        // We reset the buffer to 0, and read up to BUFF_SIZE bytes from the src_file.
        memset(buffer, 0, BUFF_SIZE); 
        bytes_read = fread(buffer, 1, BUFF_SIZE, src_file);

        // If there was an error in reading from the files, we print an error message and exit the program.
        if(ferror(src_file))
        {
            printf("Error: General failure. Exiting Program.\n");
            fclose(src_file);
            fclose(new_file);
            return 1;
        }

        // If we have read 0 bytes from the file, we know that we have reached it's end.
        // Therefore, we close the file pointers and return 0.
        if(bytes_read == 0) 
        { 
            if(verbose) { printf("Success!\n"); } // If the -v flag is used, we print a message to the user.
            fclose(src_file);
            fclose(new_file);
            return 0; 
        }

        // We now write the bytes we have read from the src_file to the new_file.
        fwrite(buffer, 1, bytes_read, new_file);

        // If there was an error in writing to the files, we print an error message and exit the program.
        if(ferror(new_file))
        {
            if(verbose) { printf("Error: General failure. Exiting Program.\n"); }
            fclose(src_file);
            fclose(new_file);
            return 1;
        }
    }
}