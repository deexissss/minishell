#include "minishell.h"

bool is_redirection_operator_inside_quotes(const char *str)
{
    bool in_single_quote = false;
    bool in_double_quote = false;

    while (*str)
    {
        if (*str == '\'' && !in_double_quote)
            in_single_quote = !in_single_quote;
        else if (*str == '"' && !in_single_quote)
            in_double_quote = !in_double_quote;
        else if (is_redirection_operator(*str) && !in_single_quote && !in_double_quote)
            return false;
        str++;
    }
    return true;
}

// Helper function to check if a character is a redirection operator
bool is_redirection_operator(char c)
{
    return (c == '<' || c == '>');
}

// Function to handle input redirection (<)
void handle_input_redirection(char *filename)
{
    while (*filename == ' ' || *filename == '\t')
        filename++;
    printf("Opening file: %s\n", filename);// Debug print
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return;
    }
    dup2(fd, STDIN_FILENO);//duplicate the fd to STDIN_FILENO: redirects stdin to read from the specified file
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
    dup2(fd, STDOUT_FILENO);
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

static int token_counter(char *command)
{
    int i = 0;
    int count = 0;

    while (command[i])
    {
        if (is_redirection_operator(command[i]))
        {
            count++;
            if ((command[i] == '>' && command[i + 1] == '>') ||
                (command[i] == '<' && command[i + 1] == '<'))
            {
                i++;
            }
        }
        i++;
    }
    return count;
}

static char *redirection_filter(char *command)
{
    int i = 0;
    int j = 0;
    int after_redirector = 0;
    char *filtered_command = malloc(strlen(command) + 1);

    if (!filtered_command)
    {
        perror("malloc");
        return NULL;
    }
    while (command[i])
    {
        if (is_redirection_operator(command[i]))
        {
            after_redirector = 1;
            filtered_command[j++] = command[i];
        }
        else if (after_redirector && (command[i] == ' ' || command[i] == '\t'))
            after_redirector = 0; // Reset the flag after skipping spaces/tabs
        else
        {
            filtered_command[j++] = command[i];
            after_redirector = 0; // Reset the flag for other characters
        }
        i++;
    }
    filtered_command[j] = '\0';
    return filtered_command;
}

char **redirection_tokenizer(char *command, int *num_tokens)
{
    char **tokens;
    char *filtered_command;
    int i = 0;
    int start = 0;
    int end = 0;
    int length;
    int segment_length;
    int token_count;

    token_count = token_counter(command);
    filtered_command = redirection_filter(command);
    if (!filtered_command)
        return NULL;

    length = strlen(filtered_command);
    tokens = malloc(sizeof(char *) * (length + 1));
    if (!tokens)
    {
        perror("malloc");
        return NULL;
    }
    i = 0;//keep track of the number of tokens and to store each token
    start = 0;
    end = 0;
    // Split the command string into tokens
    printf("Command: %s\n", filtered_command); // Debug print
    while (end <= length)
    {
        if (is_redirection_operator(filtered_command[end]) || filtered_command[end] == '\0')
        {
            segment_length = end - start;
            tokens[i] = malloc(segment_length + 1);//token for that segment
            if (!tokens[i])
            {
                perror("malloc");
                return NULL;
            }
            strncpy(tokens[i], &filtered_command[start], segment_length);
            tokens[i][segment_length] = '\0';
            printf("Token: %s\n", tokens[i]); // Debug print
            i++;
            // Handle redirection operators
            printf("Executing redirection from %d to %d\n", start, end);
            printf("Redirection segment: '%.*s'\n", end - start, &filtered_command[start]);
            start = end + 1;
            execute_redirection(filtered_command, start, end);
            printf("Redirection executed\n");
        }
        end++;
    }
    tokens[i] = NULL;
    *num_tokens = token_count;
    for (int j = 0; j < i; j++) {
        printf("Token[%d]: %s\n", j, tokens[j]); // Debug print
    }
    free(filtered_command);
    return tokens;
}

void execute_redirection(char *command, int start, int end)
{
    if (command[end] == '>')
    {
        if (command[end + 1] == '>')
        {
            printf("Found '>>' operator\n"); // Debug print
            handle_output_redirection(&command[start], true);
            end++;
        }
        else
        {
            printf("Found '>' operator\n"); // Debug print
            handle_output_redirection(&command[start], false);
        }
    }
    else if (command[end] == '<')
    {
        if (command[end + 1] == '<')
        {
            printf("Found '<<' operator\n"); // Debug print
            handle_heredoc_redirection(&command[start]);
            end++;
        }
        else
        {
            printf("Found '<' operator\n"); // Debug print
            handle_input_redirection(&command[start]);
        }
    }
}