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



/*static int check_n_option(char *inpt, int *i)
{
    if (inpt[*i] == '-' && inpt[*i + 1] == 'n' &&
        (inpt[*i + 2] == ' ' || inpt[*i + 2] == '\t' || inpt[*i + 2] == '\0'))
    {
        *i += 2;
        *i = skip_spaces(inpt, *i);
        return (0);
    }
    return (1);
}*/

/*static void print_words(char *inpt, int i)
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
}*/

/*void execute_echo(char *inpt)
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
}*/

static int skip_spaces(char *input, int i)
{
    while (input[i] == ' ' || input[i] == '\t')
        i++;
    return i;
}

static bool check_n_option(char *input, int *i)
{
    bool n_flag;
    int temp;

    n_flag = false;
    while (input[*i] == '-')
    {
        temp = *i + 1;
        while (input[temp] == 'n')
            temp++;
        if (input[temp] == ' ' || input[temp] == '\t' || input[temp] == '\0')
        {
            has_n_option = true;
            *i = skip_spaces(input, temp);
        }
        else
            break;
    }
    return has_n_option;
}

static void print_words(char *input, int i)
{
    bool first_word;
    char quote;

    first_word = true;
    while (input[i])
    {
        if (input[i] == '"' || input[i] == '\'')
        {
            quote = input[i];
            i++;
            while (input[i] && input[i] != quote)
                putchar(input[i++]);
            if (input[i] == quote)
                i++;
        }
        else if (input[i] != ' ' && input[i] != '\t')
        {
            if (!first_word)
                putchar(' ');
            first_word = false;
            while (input[i] && input[i] != ' ' && input[i] != '\t' && input[i] != '"' && input[i] != '\'')
                putchar(input[i++]);
        }
        else
            i = skip_spaces(input, i);
    }
}

void execute_echo(char *input)
{
    int i;
    bool n_option;
    
    i = 4;
    if (ft_strncmp(input, "echo", 4) != 0)
    {
        printf("error: command need to start with 'echo'\n");
        return;
    }

    if (input[i] != ' ' && input[i] != '\t')
    {
        printf("error: need a space between echo and the arguments\n");
        return;
    }
    i = skip_spaces(input, i);
    n_option = check_n_option(input, &i);
    print_words(input, i);
    if (!n_option)
        putchar('\n');
}