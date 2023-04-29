#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define MAX 128 // max length of a user's input.

// This function is responsible for handling the SIGINT signal.
void handle(int sig) { printf("\n"); }

// This function is responsible for redirecting the input/output of a command to a file.
// We have 4 options: overwrite, append, input, and none.
void redirect(char redi, char *target_file)
{
    int fd;

    // OVERWRITE: We open the file and save it's descriptor,
    // And we set the flags to read/write only, create if doesn't exist, and overwrite if exists.
    if (redi == 'o')
    {
        fd = open(target_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }

    // APPEND: We open the file and save it's descriptor,
    // And we set the flags to read/write only, create if doesn't exist, and append to last line if exists.
    else if (redi == 'a')
    {
        fd = open(target_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    }

    // INPUT: We open the file and save it's descriptor,
    // And we set the flags to read only.
    else if (redi == 'i')
    {
        fd = open(target_file, O_RDONLY);
    }

    // NONE:
    // The user didn't use redirections, so we return.
    else
    {
        return;
    }

    // If the file couldn't be opened, we throw an error message and exit.
    if (fd == -1)
    {
        perror("Error, open failed");
        exit(1);
    }

    // We duplicate the file descriptor to STDIN/STDOUT, and close the file descriptor.
    int dup;
    if (redi == 'i')
    {
        dup = dup2(fd, STDIN_FILENO);
    }
    else
    {
        dup = dup2(fd, STDOUT_FILENO);
    }

    // If the duplication failed, we throw an error message and exit.
    if (dup == -1)
    {
        perror("Error, dup2 failed");
        exit(1);
    }

    close(fd);
}

// This function is responsible for executing the user's command.
// It takes the user's input as an argument, tokenizes it and analayzes it.
void execute_command(char *input)
{
    // We will now tokenize the user's input.
    int i = 0;
    char *tokens[MAX];
    char *token = strtok(input, " ");

    while (token != NULL)
    {
        tokens[i] = token;
        token = strtok(NULL, " ");
        i++;
    }
    tokens[i] = NULL;

    // We will now check if the user used redirections.
    char redi;
    char *target_file = NULL;

    for (int i = 0; *(tokens + i) != NULL; i++)
    {
        // The user wants to redirect the output into a file of his choice, and overwrite the file's contents.
        if (strcmp(*(tokens + i), ">") == 0)
        {
            redi = 'o';
            target_file = *(tokens + i + 1);
            *(tokens + i) = NULL;
            break;
        }

        // The user wants to append the output into a file of his choice (without overwriting the file's contents).
        else if (strcmp(*(tokens + i), ">>") == 0)
        {
            redi = 'a';
            target_file = *(tokens + i + 1);
            *(tokens + i) = NULL;
            break;
        }

        // The user wants to input a file's contents into a command.
        else if (strcmp(*(tokens + i), "<") == 0)
        {
            redi = 'i';
            target_file = *(tokens + i + 1);
            *(tokens + i) = NULL;
            break;
        }
    }

    // We will now handle the redirecion.
    redirect(redi, target_file);

    // We will now check if the user wants to run a command in the background.
    int exec = execvp(tokens[0], tokens);
    if (exec == -1)
    {
        perror("Error, execvp failed");
        exit(1);
    }
}

int main()
{
    char input[MAX]; // user's input.

    // We start a loop that will run until the user enters "exit".
    while (1)
    {
        printf("shell >> ");

        fgets(input, MAX, stdin);     // We read the user's input.
        input[strlen(input) - 1] = 0; // We remove the newline character from the end of the input.

        // If the user entered "exit", we exit the program.
        if (strcmp(input, "exit") == 0)
        {
            exit(0);
        }

        // Now, we will count the number of pipes in the user's input.
        int pipe_count = 0;
        for (int i = 0; i < strlen(input); i++)
        {
            if (input[i] == '|')
            {
                pipe_count++;
            }
        }

        // If the user entered pipes, we execute the command using pipes.
        if (pipe_count > 0)
        {
            int i = 0;
            char *cmds[MAX];
            char *cmd = strtok(input, "|");

            while (cmd != NULL)
            {
                cmds[i] = cmd;
                cmd = strtok(NULL, "|");
                i++;
            }
            cmds[i] = NULL;

            // Now, we create the pipes on which we will execute each of the commands inputted by the user.
            int pipes[pipe_count][2];

            for (int i = 0; i < pipe_count; i++)
            {
                if (pipe(pipes[i]) == -1)
                {
                    perror("Error, pipe failed");
                    exit(1);
                }
            }

            // Now, we will execute each of the commands inputted by the user.
            int pid;

            for (i = 0; i < (pipe_count + 1); i++)
            {
                int pid = fork(); // We fork the process.

                // If the fork failed, we print the error message and exit.
                if (pid == -1)
                {
                    perror("Error, fork failed");
                    exit(1);
                }

                // If we are in the child process, we hadnle the SIGINT signal and execute the command.
                else if (pid == 0)
                {
                    signal(SIGINT, handle);

                    // If we are not in the first command, we redirect the input to the pipe.
                    if (i > 0)
                    {
                        int dup = dup2(pipes[i - 1][0], STDIN_FILENO);
                        if (dup == -1)
                        {
                            perror("Error, dup2 failed");
                            exit(1);
                        }
                    }

                    // If we are not in the last command, we redirect the output to the pipe.
                    if (i < pipe_count)
                    {
                        int dup = dup2(pipes[i][1], STDOUT_FILENO);
                        if (dup == -1)
                        {
                            perror("Error, dup2 failed");
                            exit(1);
                        }
                    }

                    // Now, we close unused pipes.
                    for (int j = 0; j < pipe_count; j++)
                    {
                        // If we are in the first command, we close the output pipe only.
                        if (i > 0 && j == i - 1)
                        {
                            close(pipes[j][1]);
                        }

                        // If we are in the last command, we close the input pipe only.
                        else if (i < pipe_count && j == i)
                        {
                            close(pipes[j][0]);
                        }

                        // If we are in neither the first nor the last command, we close both pipes.
                        else
                        {
                            close(pipes[j][0]);
                            close(pipes[j][1]);
                        }
                    }

                    // We execute the command.
                    execute_command(cmds[i]);
                }
            }

            // Now, we close all the pipes and wait for all the child processes to finish.
            for (i = 0; i < pipe_count; i++)
            {
                close(pipes[i][0]);
                close(pipes[i][1]);
                wait(NULL);
            }

            // We wait for the last child process to finish.
            wait(NULL);
        }

        // If no pipes were inputed by the user, we execute the command using execute().
        else
        {
            int pid = fork(); // We fork the process.

            if (pid == -1)
            {
                perror("Error, fork failed");
                exit(1);
            }

            // If we are in the child process, we hadnle the SIGINT signal and execute the command.
            else if (pid == 0)
            {
                signal(SIGINT, handle);
                execute_command(input);
            }

            // If we are in the parent process, we wait for the child process to finish.
            // When it is done, we store its exit status in the status variable.
            else
            {
                int status;
                wait(&status);
            }
        }
    }

    return 0;
}