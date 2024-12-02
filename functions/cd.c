#include "../minishell.h"

void    execute_cd(char *inpt)
{
    /*Relative Path: A path that is relative to the current directory. For example, cd folder changes to a subdirectory folder within the current directory.
    Absolute Path: A path that starts from the root directory. For example, cd /home/user/folder changes to the specified directory starting from the root.*/

    //init necessary var
    int     i;//start index after c comm
    char    *path;//buffer to hold direct path
    int     j;//index in the path buffer

    i = 2;
    j = 0;
    //skip whitespace
    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    //extract dir path
    path = malloc(sizeof(char *) * ft_strlen(inpt));
    while (inpt[i] && inpt[i] != ' ' && inpt[i] != '\t' && j < 255)
        path[j++] = inpt[i++];
    path[j] = '\0';
    //Edge case handling, eg. no dir path provided
    if (j == 0)
    {
        ft_printf("error: cd needs a directory path\n");
        return;
    }
    // Change directory
    if (chdir(path) != 0)
        perror("cd");
}