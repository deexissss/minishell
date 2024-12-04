#include "../minishell.h"

void    execute_cd(char *inpt)
{
    int     i;
    char    *path;
    int     j;

    i = 2;
    j = 0;
    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    path = malloc(sizeof(char *) * ft_strlen(inpt));
    while (inpt[i] && inpt[i] != ' ' && inpt[i] != '\t' && j < 255)
        path[j++] = inpt[i++];
    path[j] = '\0';
    if (j == 0)
    {
        ft_printf("error: cd needs a directory path\n");
        return;
    }
    if (chdir(path) != 0)
        perror("cd");
}

