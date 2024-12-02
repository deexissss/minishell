#include "../minishell.h"

void    execute_sleep(char *inpt)
{
    int     i;
    int     time;
    pid_t pid;

    i = 5;
    time = 0;
    pid = fork();
    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    if (inpt[i] == '\0')
        perror("sleep");
    while (inpt[i] >= '0' && inpt[i] <= '9')
    {
        time = time * 10 + (inpt[i] - '0');
        i++;
    }
    if (pid == -1)
        perror("fork");
    else if (pid == 0)
    {
        char time_str[20];
        snprintf(time_str, sizeof(time_str), "%d", time);
        char *args[] = {"sleep", time_str, NULL};
        if (execvp(args[0], args) == -1)
        {
            perror("execvp");
            _exit(1);
        }
    }
    else
        waitpid(pid, NULL, 0);
}
