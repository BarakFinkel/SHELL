#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_ARGS 100
#define MAX_COMMANDS 10

volatile sig_atomic_t c_pid = 0;

void handle_signal(int signal) 
{
    if (c_pid != 0) kill(c_pid, SIGINT);
}

void execute_command(char **args, int input_fd, int output_fd, int append) 
{
    pid_t pid = fork();
    
    // If fork() returns -1, an error happened, so we print the error message and exit()
    if (pid == -1) 
    {
        perror("fork");
        exit(EXIT_FAILURE);
    } 
    
    // If fork() returns 0, then we are currently in the child process.
    else if (pid == 0) 
    {
        // If input_fd is not STDIN_FILENO, then redirect stdin to input_fd using dup2
        if (input_fd != STDIN_FILENO) 
        {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }
        
        if (output_fd != STDOUT_FILENO) {
            if (append) {
                dup2(output_fd, STDOUT_FILENO);
                dup2(output_fd, STDERR_FILENO);
            } else {
                dup2(output_fd, STDOUT_FILENO);
            }
            close(output_fd);
        }
        execvp(args[0], args);
        perror(args[0]);
        exit(EXIT_FAILURE);
    } else {
        c_pid = pid;
        int status;
        waitpid(pid, &status, 0);
        c_pid = 0;
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 0) {
                fprintf(stderr, "%s exited with status %d\n", args[0], exit_status);
            }
        } else if (WIFSIGNALED(status)) {
            fprintf(stderr, "%s terminated by signal %d\n", args[0], WTERMSIG(status));
        }
    }
}

int main(int argc, char **argv) 
{
    signal(SIGINT, handle_signal);

    while (1) 
    {
        printf("> ");
        fflush(stdout);

        char input[1024];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        if (strlen(input) > 0 && input[strlen(input)-1] == '\n') {
            input[strlen(input)-1] = '\0';
        }

        char *commands[MAX_COMMANDS][MAX_ARGS];
        int num_commands = 0;
        char *ptr = strtok(input, "|");
        while (ptr != NULL && num_commands < MAX_COMMANDS) {
            int num_args = 0;
            char *arg = strtok(ptr, " ");
            while (arg != NULL && num_args < MAX_ARGS) {
                commands[num_commands][num_args++] = arg;
                arg = strtok(NULL, " ");
            }
            commands[num_commands++][num_args] = NULL;
            ptr = strtok(NULL, "|");
        }

        int fds[2][2];
        int input_fd = STDIN_FILENO;
        for (int i = 0; i < num_commands; i++) {
            int output_fd = STDOUT_FILENO;
            int append = 0;
            char *arg = commands[i][0];
            while (arg != NULL && *arg != '\0') {
                if (*arg == '>') {
                    arg++;
                    if (*arg == '>') {
                        append = 1;
                        arg++;
                    }
                    while (*arg == ' ') {
                        arg++;
                    }
                    output_fd = open(arg, append ? O_WRONLY | O_CREAT | O_APPEND : O_WRONLY | O_CREAT | O_TRUNC, 0666);
                    if (output_fd == -1) 
                    {
                        perror(arg);
                        exit(EXIT_FAILURE);
                    }
                    break;
                }
                arg++;
            }
        
            if (i < num_commands - 1) {
                if (pipe(fds[i % 2]) == -1) {
                    perror("pipe");
                    exit(EXIT_FAILURE);
                }
                output_fd = fds[i % 2][1];
            }
            else {
            fds[i % 2][0] = -1;
            }

            execute_command(commands[i], input_fd, output_fd, append);

            if (input_fd != STDIN_FILENO) {
                close(input_fd);
            }
            
            if (output_fd != STDOUT_FILENO) {
                close(output_fd);
            }
            
            input_fd = fds[i % 2][0];
        }
    }

return 0;

}


