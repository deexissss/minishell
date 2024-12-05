#include "../minishell.h"

void    execute_pwd()
{
    char cwd[4096];
    pid_t pid;

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