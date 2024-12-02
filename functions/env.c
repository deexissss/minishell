#include "../minishell.h"

extern char **environ;;

/*void    execute_env(void)
{
    //function to print the environment variables
    // Initialization
    int i;
    pid_t pid;
    // Fork a new process
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    else if (pid == 0)
    {
        // Child process to print the environment variables
        i = 0;
        while (environ[i] != NULL)
        {
            ft_printf("%s\n", environ[i]);
            i++;
        }
        exit(0);
    }
    else
    {
        // Wait for the child process to finish
        if (waitpid(pid, NULL, 0) == -1)
        {
            perror("waitpid");
        }
    }
}*/

void print_environment_variables(void)
{
    int i = 0;
    while (environ[i] != NULL)
    {
        printf("%s\n", environ[i]);
        i++;
    }
}

void execute_env(void)
{
    pid_t pid;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    else if (pid == 0)
    {
        print_environment_variables();
        exit(0);
    }
    else
    {
        if (waitpid(pid, NULL, 0) == -1)
            perror("waitpid");
    }
}