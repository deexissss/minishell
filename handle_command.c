#include "minishell.h"
//check if the command exist
int correct_command(char **args)
{
    if (!args || !args[0])
    {
        printf("error: command not found\n");
        if (args)
            free(args);
        return 0;
    }
    return 1;
}
//give the path to the command by adding /bin/ before 
char *command_path(char *command_name)
{
    char *path;

    path = ft_strjoin("/bin/", command_name);
    if (!path)
        perror("malloc");
    return path;
}
//execute the command with the path (/bin/ls/ for ex) + the argument
void execute_command(char *path, char **args)
{
    if (execve(path, args, NULL) == -1)
    {
        perror("minishell");
        exit(EXIT_FAILURE);
    }
}
//split the command to have the command + the flag + the args
void handle_external_command(char *command)
{
    char **args;
    char *path;
    pid_t pid;
    int status;

    args = ft_split(command, ' ');
    if (!correct_command(args))
        return;
    path = command_path(args[0]);
    if (!path)
    {
        free(args);
        return;
    }
    pid = fork();
    if (pid == 0)
        execute_command(path, args);
    else if (pid > 0)
        waitpid(pid, &status, 0);
    else
        perror("fork");
    free(path);
    free(args);
}
//check if the command is one of our builtin -> if not execute function to handle all the other functions
void ft_checker(char *command)
{
    if (memcmp(command, "cd", 2) == 0)
        execute_cd(command);
    else if (memcmp(command, "env", 3) == 0)
        execute_env();
    else if (memcmp(command, "exit", 4) == 0)
        execute_exit(command);
    else if (memcmp(command, "echo", 4) == 0)
        execute_echo(command);
    else if (memcmp(command, "pwd", 3) == 0)
        execute_pwd();
    else if (memcmp(command, "unset", 5) == 0)
        execute_unset(command);
    else if (memcmp(command, "export", 6) == 0)
        execute_export(command);
    else
        handle_external_command(command);
}
