#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFF_SIZE 1024

int verbose = 0;
int case_ignore = 0;

int main(int argc, char *argv[])
{
    // We check if the number of arguments is correct.
    // If not, we print an error message and exit the program.
    if (argc != 3)
    {
        printf("Error: received less variables than expected. \n");
        printf("Please execute the file as following: ./cmp <file1> <file2> (-v) (-i)\n");
        return 1;
    }

    for(int i=3; i < argc; i++)
    {
        if (argv[i] == "-v") ( verbose = 1 );
        else if (argv[i] == "-i") ( case_ignore = 1 );
        else { printf("Error: Flag is not recognised. Exiting Program. \n"); return 1; }
    }

    FILE *f1 = fopen(argv[1], "r");
    FILE *f2 = fopen(argv[2], "r");
    
    if (f1 == NULL || f2 == NULL)
    {
        printf("Error: A file or both could not be opened. Exiting Program. \n");
        return 1;
    }
    
    int bytes_read_1, bytes_read_2;
    unsigned char buff_1[BUFF_SIZE], buff_2[BUFF_SIZE];
    
    // We now begin a while loop which will stop as soon as the files are done reading and are the same or if they are not the same.
    while (1)
    {
        // We read the files into the buffers and save the number of bytes read from each file.
        bytes_read_1 = fread(buff_1, 1, BUFF_SIZE, f1);
        bytes_read_2 = fread(buff_2, 1, BUFF_SIZE, f2);
        
        // If the number of bytes read from each file is different, we know that the files are not the same.
        // Therefore, we close the file pointers and return 1.
        if (bytes_read_1 != bytes_read_2) 
        { 
            if (verbose) { printf("Distinct.\n"); }  // If the -v flag is used, we print a message to the user.
            fclose(f1);
            fclose(f2);
            return 1; 
        }

        // If the -i flag is used, we view all uppercase letters as lowercase.
        if (case_ignore == 1)
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
        
        // If the number of bytes read is less than the buffer size, we know that we have reached the end of the file.
        // If non of the previous conditions were met, we know that the files are the same. 
        // Therefore, we close the file pointers and we return 0.
        if (bytes_read_1 < BUFF_SIZE)
        {
            if (verbose) { printf("Equal.\n"); }
            fclose(f1);
            fclose(f2);
            return 0;
        }
    }

}