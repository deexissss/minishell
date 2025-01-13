#include "../minishell.h"

extern char **environ;;


void print_environment_variables(void)
{
    int i;

    i = 0;
    while (environ[i] != NULL)
    {
        printf("%s\n", environ[i]);
        i++;
    }
}

void validate_env_command(char *command)
{
    int i;

    i = 0;
    while (command[i] >= 'a' && command[i] <= 'z' && i < 3)
        i++;
    if (command[i] >= 'a' && command[i] <= 'z')
    {
        printf("error: command does not exist\ndid you mean env ?\n");
        exit_status = 127;
        return;
    }
    else if (command[i] != '\0')
    {
        printf("error: env dont take any argument\n");
        exit_status = 1;
        return;
    }
}

void execute_env(char *command)
{
    pid_t pid;

    exit_status = 0;
    validate_env_command(command);
    if (exit_status != 0)
        return;

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

/*void execute_env(char *command)
{
    pid_t pid;
    int   i;

    exit_status = 0;
    i = 0;
    while (command[i] >= 'a' && command[i] <= 'z' && i < 3)
        i++;
    if (command[i] >= 'a' && command[i] <= 'z')
    {
        printf("error: command does not exist\ndid you mean env ?\n");
        exit_status = 127;
        return;
    }
    else if (command[i] != '\0')
    {
        printf("error: env dont take any argument\n");
        exit_status = 1;
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
}*/
