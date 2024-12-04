#include "../minishell.h"

extern char **environ;;


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