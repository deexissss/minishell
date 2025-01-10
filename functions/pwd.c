#include "../minishell.h"

void    execute_pwd(char *command)
{
    char cwd[4096];
    pid_t pid;
    int     i;

    exit_status = 0;
    i = 0;
    while (command[i] >= 'a' && command[i] <= 'z' && i < 3)
        i++;
    if (command[i] >= 'a' && command[i] <= 'z')
    {
        printf("error: command does not exit\ndid you mean pwd ?\n");
        exit_status = 127;
        return;
    }
    else if (command[i] != '\0')
    {
        printf("error: pwd does not take any argument\n");
        exit_status = 1;
        return;
    }
    pid = fork();
    if (pid == -1)
        perror("fork");
    else if (pid == 0)
    {
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            printf("%s\n", cwd);
        else
        {
            perror("getcwd");
            exit(1);
        }
        exit(0);
    }
    else
        waitpid(pid, NULL, 0);
}

char    *execute_pwdmain(void)
{
    static char cwd[4096];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
        return cwd;
    else
    {
        perror("getcwd");
        return NULL;
    }
}