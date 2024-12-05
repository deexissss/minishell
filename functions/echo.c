#include "../minishell.h"

void    execute_echo(char *inpt)
{
    int     i;
    int     n;

    i = 4;
    n = 1;
    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    if (inpt[i] == '-' && inpt[i + 1] == 'n' && (inpt[i + 2] == ' ' || 
        inpt[i + 2] == '\t' || inpt[i + 2] == '\0'))
    {
        n = 0;
        i += 2;
        while(inpt[i] == ' ' || inpt[i] == '\t')
            i++;
    }
    /*while (inpt[i])
    {
        write(1, &inpt[i], 1);
        i++;
    }*/
    printf("%s", inpt + i);
    if(n)
        printf("\n");
}
