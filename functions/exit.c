#include "../minishell.h"

void    skip_space_and_sign(char *input, int *i, int *sign)
{
    while (input[*i] == ' ' || input[*i] == '\t')
        (*i)++;
    if (input[*i] == '-' && input[*i + 1] != '-')
    {
        *sign = -1;
        (*i)++;
    }
}

int give_status(char *input, int *i)
{
    int status;

    status = 0;
    while (input[*i])
    {
        if (input[*i] >= '0' && input[*i] <= '9')
            status = status * 10 + (input[(*i)++] - '0');
        else if (input[*i] >= 'a' && input[*i] <= 'z')
            status = status * 10 + (input[(*i)++] - 'a');
        else if (input[*i] >= 'A' && input[*i] <= 'Z')
            status = status * 10 + (input[(*i)++] - 'A');
    }
    return (status);
}

int     count_arg(char *inpt)
{
    int count;
    int i;

    i = 0;
    count = 0;
    while (inpt[i])
    {
        if (inpt[i] != ' ' && inpt[i] != '\t')
        {
            count++;
            while (inpt[i] && inpt[i] != ' ' && inpt[i] != '\t')
                i++;
        }
        else
            i++;
    }
    return (count);
}

void    execute_exit(char *input)
{
    int i;
    int sign;
    int status;
    int count;

    i = 4;
    sign = 1;
    count = count_arg(input);
    skip_space_and_sign(input, &i, &sign);
    /*if ((input[i] == '-' && input[i + 1] == '-') ||
        (input[i] == '+' && input[i + 1] == '+'))
    {
        printf("error: numeric argument required\n");
        return ;
    }*/
    if (count > 2)
    {
        printf("error: too many arguments\n");
        exit_status = 1;
        return ;
    }
    status = give_status(input, &i);
    if (status >= 0)
    {
        printf("exit\n");
        exit(status * sign);
    }
}

