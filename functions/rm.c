#include "../minishell.h"

static void  skip_space_and_sign(char *input, int *i)
{
    while (input[*i] == ' ' || input[*i] == '\t')
        (*i)++;
}

static void    check_pid(pid_t pid)
{
    if (pid == -1)
    {
        perror("fork");
        return;
    }
}

/*void execute_rm(char *inpt)
{
    int i;
    pid_t pid;
    char filename[256];
    int j;

    i = 2;
    j = 0;
    skip_space_and_sign2(inpt, &i);
    if (inpt[i] == '\0')
    {
        write(1, "error: rm needs a filename\n", 27);
        return;
    }
    pid = fork();
    check_pid2(pid);
    if (pid == 0)
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
}*/


static void get_filename(char *inpt, char *filename, int *i)
{
    int j;

    j = 0;
    while (inpt[*i] && inpt[*i] != ' ' && inpt[*i] != '\t' && j < 255)
        filename[j++] = inpt[(*i)++];
    filename[j] = '\0';
}

static void handle_child_rm(char *filename)
{
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

void execute_rm(char *inpt)
{
    int     i;
    pid_t   pid;
    char    filename[256];

    i = 2;
    skip_space_and_sign(inpt, &i);
    if (inpt[i] == '\0')
    {
        write(1, "error: rm needs a filename\n", 27);
        return ;
    }
    pid = fork();
    check_pid(pid);
    if (pid == 0)
    {
        get_filename(inpt, filename, &i);
        handle_child_rm(filename);
    }
    else
        waitpid(pid, NULL, 0);
}
