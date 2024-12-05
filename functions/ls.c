#include "../minishell.h"

void    check_pid(pid_t pid)
{
    if (pid == -1)
    {
        perror("fork");
        return;
    }
}
void    open_dir(DIR *dir)
{
    if (dir == NULL)
    {
        perror("opendir");
        exit(1);
    }
}

void execute_ls()
{
    DIR *dir;
    struct dirent *entry;
    pid_t pid;

    pid = fork();
    check_pid(pid);
    if (pid == 0)
    {
        dir = opendir(".");
        open_dir(dir);
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_name[0] != '.')
            {
                write(1, entry->d_name, ft_strlen(entry->d_name));
                write(1, "   ", 3);
            }
        }
        closedir(dir);
        exit(0);
    }
    else
        waitpid(pid, NULL, 0);
    printf("\n");
}

void    execute_lsl()
{
    char *args[] = {"/bin/ls", "-l", NULL};
    pid_t pid = fork();

    if (pid == -1)
        perror("fork");
    else if (pid == 0)
    {
        if (execve(args[0], args, NULL) == -1)
        {
            perror("execve");
            _exit(1);
        }
    }
    else
        waitpid(pid, NULL, 0);
}
