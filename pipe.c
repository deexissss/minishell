#include "minishell.h"

/*splits string command into segments based on the pipe (|) character, 
storing each segment in an array of strings.*/
char **pipe_tokenizer(char *command, int *num_commands)
{
    char **commands;
    int i;
    int start;
    int end;
    int length;
    int segment_length;
    int command_count;

    i = 0;
    start = 0;
    end = 0;
    length = ft_strlen(command);
    commands = malloc(sizeof(char *) * (length + 1));
    command_count = 1;
    while (command[i])
    {
        if (command[i] == '|')
            command_count++;
        i++;
    }
    if (!commands)
    {
        perror("malloc");
        return NULL;
    }
    i = 0;
    while (end <= length)
    {
        if (command[end] == '|' || command[end] == '\0')
        {
            //calculates the length of the current segment and allocates memory
            segment_length = end - start;
            commands[i] = malloc(segment_length + 1);//allocates memory for the segment specifically
            if (!commands[i])
            {
                perror("malloc");
                exit(EXIT_FAILURE);
            }
            //copies the segment into the commands array
            ft_strncpy(commands[i], &command[start], segment_length);//copied from command[start] to command[end]/segment_length
            commands[i][segment_length] = '\0';//this is new, but basically it adds a null terminator to the end of the segment and not the end of the command
            i++;
            start = end + 1;//to point to the beginning of the next segment
        }
        end++;
    }
    commands[i] = NULL;
    *num_commands = i;//updates the number of commands
    return commands;
}

/*creates a pipeline where each command's output is connected to the next command's input. 
This ensures that the commands are executed in sequence, 
with the output of one command serving as the input to the next*/
void execute_pipeline(char **commands, int num_commands)
{
    int pipefd[2]; //array to hold the file descriptors for the pipe
    pid_t pid;//stores the process id returned by fork
    int fd_in;//stores the file descriptor for the input of the next command
    int i;

    fd_in = 0;
    i = 0;
    while(i < num_commands)
    {
        //creates a pipe
        if (pipe(pipefd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        //forks the process
        if ((pid = fork()) == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        //child process
        if (pid == 0)
        {
            dup2(fd_in, 0); //redirects the input to the previous command's output
            if (i < num_commands - 1)
                dup2(pipefd[1], 1); //changes the output to the end of the pipe only if it's not the last command
            close(pipefd[0]);
            close(pipefd[1]);
            ft_checker(commands[i]);//use ft_checker to handle the command
            exit(EXIT_SUCCESS); //ensure the child process exits after executing the command
        }
        //parent process
        else
        {
            waitpid(pid, NULL, 0); //wait for the specific child process to finish
            close(pipefd[1]);
            fd_in = pipefd[0]; //save the input for the next command
        }
        i++;
    }
}
