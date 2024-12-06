#include "../minishell.h"

/*void    execute_echo(char *inpt)
{
    int     i;
    int     n;
    int     first_word;

    i = 4;
    n = 1;
    if (inpt[i] != ' ' && inpt[i] != '\t')
    {
        printf("error: need a space between echo and the argument\n");
        return ;
    }
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
    first_word = 1;
    while (inpt[i] != '\0')
    {
        if (inpt[i] != ' ' && inpt[i] != '\t')
        {
            if (!first_word)
                printf(" ");
            first_word = 0;
            while (inpt[i] != '\0' && inpt[i] != ' ' && inpt[i] != '\t')
            {
                putchar(inpt[i]);
                i++;
            }
        }
        else
        {
            while (inpt[i] == ' ' || inpt[i] == '\t')
                i++;
        }
    }
    if(n)
        printf("\n");
}*/

static int skip_spaces(char *inpt, int i)
{
    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    return (i);
}

static int check_n_option(char *inpt, int *i)
{
    if (inpt[*i] == '-' && inpt[*i + 1] == 'n' &&
        (inpt[*i + 2] == ' ' || inpt[*i + 2] == '\t' || inpt[*i + 2] == '\0'))
    {
        *i += 2;
        *i = skip_spaces(inpt, *i);
        return (0);
    }
    return (1);
}

static void print_words(char *inpt, int i)
{
    int first_word;

    first_word = 1;
    while (inpt[i])
    {
        if (inpt[i] != ' ' && inpt[i] != '\t')
        {
            if (!first_word)
                printf(" ");
            first_word = 0;
            while (inpt[i] && inpt[i] != ' ' && inpt[i] != '\t')
                putchar(inpt[i++]);
        }
        else
            i = skip_spaces(inpt, i);
    }
}

void execute_echo(char *inpt)
{
    int i;
    int n;

    i = 4;
    if (inpt[i] != ' ' && inpt[i] != '\t')
    {
        printf("error: need a space between echo and the argument\n");
        return ;
    }
    i = skip_spaces(inpt, i);
    n = check_n_option(inpt, &i);
    print_words(inpt, i);
    if (n)
        printf("\n");
}