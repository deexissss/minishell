#include "../minishell.h"

void  skip_space_and_sign(char *input, int *i)
{
    while (input[*i] == ' ' || input[*i] == '\t')
        (*i)++;
}

void    check_pid(pid_t pid)
{
    if (pid == -1)
    {
        perror("fork");
        return;
    }
}

void execute_rm(char *inpt)
{
    int i = 2;
    pid_t pid;
    char filename[70000];
    int j = 0;

    skip_space_and_sign(inpt, &i);
    if (inpt[i] == '\0')
    {
        write(1, "error: rm needs a filename\n", 27);
        return;
    }
    pid = fork();
    check_pid(pid);
    else if (pid == 0)
    {
        while (inpt[i] && inpt[i] != ' ' && inpt[i] != '\t' && j < 255)
            filename[j++] = inpt[i++];
        filename[j] = '\0';

        if (access(filename, F_OK) == 0)
        {
            if (unlink(filename) == -1)
            {
                perror("rm");
                exit(1);
            }
        }
        else
        {
            perror("rm");
            exit(1);
        }
        exit(0);
    }
    else
        waitpid(pid, NULL, 0);
}
