#include <stdio.h>
#include <string.h>

#define BUFF_SIZE 1024

int verbose = 0;
int ignore = 0;

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
    for(int i=3; i < argc; i++)
    {
        if (!strcmp(argv[i], "-v"))      ( verbose = 1 );
        else if (!strcmp(argv[i], "-i")) ( ignore  = 1 );
        else { printf("Error: Flag is not recognised. Exiting Program. \n"); return 1; }
    }

    // We set file pointers to the files given by the user.
    FILE *f1 = fopen(argv[1], "r");
    FILE *f2 = fopen(argv[2], "r");
    
    // If the files could not be opened propperly, we print an error message and exit the program.
    if (f1 == NULL || f2 == NULL)
    {
        printf("Error: A file or both could not be opened. Exiting Program. \n");
        return 1;
    }
    
    // Now, we set variables to count bytes read from each file, and a buffer to read the bytes into.
    int bytes_read_1, bytes_read_2;
    unsigned char buff_1[BUFF_SIZE], buff_2[BUFF_SIZE];
    
    // We now begin a while loop which will stop as soon as the files are done reading and are the same or if they are not the same.
    while (1)
    {
        // We set the buffers to 0, and read BUFF_SIZE bytes from each file into the buffers.
        memset(buff_1, 0, BUFF_SIZE);
        memset(buff_2, 0, BUFF_SIZE);
        bytes_read_1 = fread(buff_1, 1, BUFF_SIZE, f1);
        bytes_read_2 = fread(buff_2, 1, BUFF_SIZE, f2);
        
        // If there was an error in reading from the files, we print an error message and exit the program.
        if (ferror(f1) || ferror(f2))
        {
            printf("Error: An error occured while reading from the files. Exiting Program. \n");
            fclose(f1);
            fclose(f2);
            return 1;
        }

        // If the number of bytes read from each file is different, we know that the files are not the same.
        // Therefore, we close the file pointers and return 1.
        if (bytes_read_1 != bytes_read_2) 
        { 
            if (verbose) { printf("Distinct.\n"); }  // If the -v flag is used, we print a message to the user.
            fclose(f1);
            fclose(f2);
            return 1; 
        }

        // If we have read 0 bytes from each file, we know that we have reached their end.
        // Therefore, we close the file pointers and return 0.
        if (bytes_read_1 == 0 && bytes_read_2 == 0) 
        { 
            if (verbose) { printf("Equal.\n"); } // If the -v flag is used, we print a message to the user.
            fclose(f1);
            fclose(f2);
            return 0; 
        }

        // If the -i flag is used, we view all uppercase letters as lowercase.
        // Therefore, we convert all uppercase letters in the buffer to lowercase.
        if (ignore == 1)
        {
            for (int i=0; i < bytes_read_1; i++)
            {
                if (buff_1[i] >= 'A' && buff_1[i] <= 'Z') { buff_1[i] += 32; }
                if (buff_2[i] >= 'A' && buff_2[i] <= 'Z') { buff_2[i] += 32; }
            }
        }
        
        // If the buffers are the same, we continue to the next iteration of the loop.
        // Otherwise, we close the file pointers and return 1.
        if (memcmp(buff_1, buff_2, bytes_read_1) != 0) 
        { 
            if (verbose) { printf("Distinct.\n"); } 
            fclose(f1);
            fclose(f2);
            return 1; 
        }
    }
}