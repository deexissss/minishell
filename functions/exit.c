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
        else
        {
            printf("error: numeric argument required\n");
            return (-1);
        }
    }
    return (status);
}

void    execute_exit(char *input)
{
    int i;
    int sign;
    int status;

    i = 4;
    sign = 1;
    skip_space_and_sign(input, &i, &sign);
    if ((input[i] == '-' && input[i + 1] == '-') ||
        (input[i] == '+' && input[i + 1] == '+'))
    {
        printf("error: numeric argument required\n");
        return ;
    }
    status = give_status(input, &i);
    if (status >= 0)
    {
        printf("exit\n");
        exit(status * sign);
    }
}

