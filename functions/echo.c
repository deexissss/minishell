#include "../minishell.h"

void    execute_echo(char *inpt)
{
    int     i;
    int     n;//flag to control newline printing

    i = 4;
    n = 1; //newline character will be printed by default
    //skip whitespace
    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    //check for -n flag
    //(inpt[i + 2] == ' ' || ...) ensures that the -n flag is a standalone flag and not part of the text to be echoed
    if (inpt[i] == '-' && inpt[i + 1] == 'n' && (inpt[i + 2] == ' ' || 
        inpt[i + 2] == '\t' || inpt[i + 2] == '\0'))
    {
        n = 0;//if detected n set to 0 to disable newline printing
        i += 2;// move index past -n
        //skip any additional whitespace after -n flag
        while(inpt[i] == ' ' || inpt[i] == '\t')
            i++;
    }
    while (inpt[i])
    {
        write(1, &inpt[i], 1);
        i++;
    }
    //conditional newline, if n = 1 print newline, if -n flag was set no newline
    if(n)
        write(1, "\n", 1);
}