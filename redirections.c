#include "minishell.h"

// Function to handle input redirection (<)
void handle_input_redirection(char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return;
    }
    if (dup2(fd, STDIN_FILENO) == -1)//duplicate the fd to STDIN_FILENO: redirects stdin to read from the specified file
    {
        perror("dup2");
        close(fd);
        return;
    }
    close(fd);
}

// Function to handle output redirection (> + >>)
/*The function takes two parameters: 
filename, representing the name of the file to which the output should be redirected, 
and append, a boolean flag indicating whether the output should be appended to the file 
or if the file should be truncated before writing.
- 0644/file permission means:
The owner can read and write the file.
The group can only read the file.
Others can only read the file.*/
void handle_output_redirection(char *filename, bool append)
{
    int fd;
    while (*filename == ' ' || *filename == '\t')
        filename++;
    if (append)
        /*open the specified file in write-only mode, 
        creating the file if it does not exist, 
        and appending to the file if it does exist.*/
        fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        /*attempts to open the file in write-only mode, 
        creating the file if it does not exist, 
        and truncating the file (i.e., clearing its contents) if it does exist*/
        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        return;
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        return;
    }
    close(fd);
}

// Function to handle heredoc redirection (<<)
/*This operator allows the user to specify a delimiter 
and then provide input until a line containing the delimiter is encountered.*/
void handle_heredoc_redirection(char *delimiter)
{
    char *line;// hold each line of input
    int pipefd[2];// hold the file descriptors for a pipe

    while (*line == ' ' || *line == '\t')
        line++;
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return;
    }
    while (1)
    {
        line = readline("> ");// prints a > prompt and reads a line of input from the user
        if (strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        //the function writes the line to the write end of the pipe
        write(pipefd[1], line, strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }
    close(pipefd[1]);//close the write end of the pipe
    dup2(pipefd[0], STDIN_FILENO);//duplicate the read end of the pipe to STDIN_FILENO, effectively redirecting stdin to read from the pipe
    close(pipefd[0]);
}
// Function to handle the command or arguments + concatenation of arguments with space
void handle_command_or_args(char *token, char **cmd, char **args)
{
    if (*cmd == NULL)
    {
        *cmd = ft_strdup(token);
    }
    else
    {
        if (*args == NULL)
            *args = ft_strdup(token);
        else
        {
            char *temp = ft_strjoin(*args, " ");
            char *new_args = ft_strjoin(temp, token);
            free(temp);
            free(*args);
            *args = new_args;
        }
    }
}
// Function to handle the token
void handle_token(char *token, char **cmd, char **args)
{
    if (ft_strcmp(token, ">") == 0)
    {
        token = ft_strtok(NULL, " ");
        if (token != NULL)
            handle_output_redirection(token, false);
    }
    else if (ft_strcmp(token, ">>") == 0)
    {
        token = ft_strtok(NULL, " ");
        if (token != NULL)
            handle_output_redirection(token, true);
    }
    else if (ft_strcmp(token, "<") == 0)
    {
        token = ft_strtok(NULL, " ");
        if (token != NULL)
            handle_input_redirection(token);
    }
    else
         handle_command_or_args(token, cmd, args);
}
// Function to execute the command with redirection
void execute_command_with_redirection(char *cmd, char *args)
{
    if (cmd != NULL)
    {
        size_t full_command_length = ft_strlen(cmd) + (args ? ft_strlen(args) : 0) + 2;//checks if args is not null(if so return 0), then adds the length of args to the length of cmd
        char *full_command = malloc(full_command_length);
        ft_strcpy(full_command, cmd);
        if (args)
        {
            ft_strlcat(full_command, " ", full_command_length);
            ft_strlcat(full_command, args, full_command_length);
        }
        ft_checker(full_command); // Use ft_checker to execute the command
        free(full_command);
        free(cmd);
        if (args)
            free(args);
    }
}
// Function to execute redirection
void execute_redirection(char *command)
{
    char *token;
    char *cmd = NULL;
    char *args = NULL;

    token = ft_strtok(command, " ");
    while (token != NULL)
    {
        handle_token(token, &cmd, &args);
        token = ft_strtok(NULL, " ");
    }
    // Execute the command after handling redirections
    if (cmd != NULL)
        execute_command_with_redirection(cmd, args);
    else
        printf("Error: No valid command found to execute.\n");
}