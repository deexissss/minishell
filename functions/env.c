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

void execute_env(char *command)
{
    pid_t pid;
    int   i;

    i = 0;
    while (command[i] >= 'a' && command[i] <= 'z' && i < 3)
        i++;
    if (command[i] >= 'a' && command[i] <= 'z')
    {
        printf("error: command does not exist\ndid you mean env ?\n");
        return;
    }
    else if (command[i] != '\0')
    {
        printf("error: env dont take any argument\n");
        return;
    }
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