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

/*void execute_redirection(char *command)
{
    char *token;

    token = ft_strtok(command, " ");
    while (token != NULL)
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
        token = ft_strtok(NULL, " ");
    }
    return;
}*/

/*void execute_redirection(char *command)
{
    char *token;

    printf("Executing redirection for command: %s\n", command);
    token = ft_strtok(command, " ");
    while (token != NULL)
    {
        printf("Token: %s\n", token);
        if (ft_strcmp(token, ">") == 0)
        {
            token = ft_strtok(NULL, " ");
            if (token != NULL)
            {
                printf("Output redirection to: %s (overwrite)\n", token);
                handle_output_redirection(token, false);
            }
        }
        else if (ft_strcmp(token, ">>") == 0)
        {
            token = ft_strtok(NULL, " ");
            if (token != NULL)
            {
                printf("Output redirection to: %s (append)\n", token);
                handle_output_redirection(token, true);
            }
        }
        else if (ft_strcmp(token, "<") == 0)
        {
            token = ft_strtok(NULL, " ");
            if (token != NULL)
            {
                printf("Input redirection from: %s\n", token);
                handle_input_redirection(token);
            }
        }
        token = ft_strtok(NULL, " ");
    }
}*/

char *concatenate_arguments(char *cmd, char *args, const char *token)
{
    if (cmd == NULL)
        cmd = strdup(token);
    else
    {
        if (args == NULL)
            args = strdup(token);
        else
        {
            char *temp = malloc(strlen(args) + strlen(token) + 2);
            strcpy(temp, args);
            strcat(temp, " ");
            strcat(temp, token);
            free(args);
            args = temp;
        }
    }
    return args;
}

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
    else if (ft_strcmp(token, "<<") == 0)
    {
        token = ft_strtok(NULL, " ");
        if (token != NULL)
            handle_heredoc_redirection(token);
    }
    else
    {
        *args = concatenate_arguments(*cmd, *args, token);
        if (*cmd == NULL)
        {
            *cmd = *args;
            *args = NULL;
        }
    }
}

void execute_command_with_redirection(char *cmd, char *args)
{
    if (cmd != NULL)
    {
        size_t full_command_length = strlen(cmd) + (args ? strlen(args) : 0) + 2;
        char *full_command = malloc(full_command_length);
        strcpy(full_command, cmd);
        if (args)
        {
            strcat(full_command, " ");
            strcat(full_command, args);
        }
        ft_checker(full_command); // Use ft_checker to execute the command
        free(full_command);
        free(cmd);
        if (args)
            free(args);
    }
}

void execute_redirection(char *command)
{
    char *token;
    char *cmd = NULL;
    char *args = NULL;
    char *original_command = strdup(command); // Save the original command

    token = ft_strtok(command, " ");
    while (token != NULL)
    {
        handle_token(token, &cmd, &args);
        token = ft_strtok(NULL, " ");
    }

    // Execute the command after handling redirections
    execute_command_with_redirection(cmd, args);

    free(original_command);
}