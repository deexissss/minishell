#include "../minishell.h"

void execute_touch(char *inpt)
{
    int i = 5;
    pid_t pid;
    char filename[70000];
    int j = 0;

    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    if (inpt[i] == '\0')
    {
        write(1, "error: touch needs a filename\n", 30);
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
        while (inpt[i] && inpt[i] != ' ' && inpt[i] != '\t' && j < 255)
            filename[j++] = inpt[i++];
        filename[j] = '\0';
        int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
        {
            perror("touch");
            exit(1);
        }
        close(fd);
        exit(0);
    }
    else
        waitpid(pid, NULL, 0);
}
