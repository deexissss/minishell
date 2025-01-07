#include "minishell.h"

// Helper function to check if a pipe is inside quotes
bool is_pipe_inside_quotes(const char *str)
{
    bool in_single_quote = false;
    bool in_double_quote = false;

    while (*str)
    {
        if (*str == '\'' && !in_double_quote)
            in_single_quote = !in_single_quote;
        else if (*str == '"' && !in_single_quote)
            in_double_quote = !in_double_quote;
        else if (*str == '|' && !in_single_quote && !in_double_quote)
            return false;
        str++;
    }
    return true;
}

/*splits string command into segments based on the pipe (|) character, 
storing each segment in an array of strings.*/
char **pipe_tokenizer(char *command, int *num_commands)
{
    int i = 0;
    char *token;
    char *command_copy;
    char **commands;

    // Duplicate the command string to avoid modifying the original
    command_copy = strdup(command);
    if (!command_copy)
    {
        perror("strdup");
        return NULL;
    }
    // Allocate memory for the commands array
    commands = malloc((strlen(command) / 2 + 1) * sizeof(char *));
    if (!commands)
    {
        perror("malloc");
        free(command_copy);
        return NULL;
    }
    // Tokenize the command string
    token = ft_strtok(command_copy, "|");
    while (token != NULL)
    {
        commands[i] = strdup(token); // Duplicate the token and store it in the commands array
        if (!commands[i])
        {
            perror("strdup");
            while (i > 0)
                free(commands[--i]);
            free(commands);
            free(command_copy);
            return NULL;
        }
        i++;
        token = ft_strtok(NULL, "|");
    }
    commands[i] = NULL;
    *num_commands = i; // Update the number of commands
    free(command_copy);
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