#include "../minishell.h"

static int check_filename(char *inpt, int *i)
{
    while (inpt[*i] == ' ' || inpt[*i] == '\t')
        (*i)++;
    if (inpt[*i] == '\0')
    {
        write(1, "error: touch needs a filename\n", 30);
        return (0);
    }
    return (1);
}
static void    check_pid(pid_t pid)
{
    if (pid == -1)
    {
        perror("fork");
        return;
    }
}
static void check_fd(int fd)
{
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }
}
static void get_filename(char *inpt, char *filename, int *i)
{
    int j;

    j = 0;
    while (inpt[*i] && inpt[*i] != ' ' && inpt[*i] != '\t' && j < 255)
        filename[j++] = inpt[(*i)++];
    filename[j] = '\0';
}

void execute_touch(char *inpt)
{
    int i;
    pid_t pid;
    char filename[256];
    int fd;

    i = 5;
    if (!check_filename(inpt, &i))
        return;
    pid = fork();
    check_pid(pid);
    if (pid == 0)
    {
        get_filename(inpt, filename, &i);
        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        check_fd(fd);
        close(fd);
        exit(0);
    }
    else
        waitpid(pid, NULL, 0);
}