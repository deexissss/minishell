#include "../minishell.h"

extern char **environ;;

static void check_var(char *input, int *i)
{
    while (input[*i] == ' ' || input[*i] == '\t')
        (*i)++;
    if (input[*i] == '\0')
    {
        ft_printf("error: unset needs a variable name\n");
        return;
    }
}
void execute_unset(char *inpt)
{
    int i;
    char varname[256];
    long unsigned int j;

    i = 6;
    j = 0;
    check_var(inpt, &i);
    while (inpt[i] && inpt[i] != ' ' && inpt[i] != '\t' && j < sizeof(varname) - 1)
        varname[j++] = inpt[i++];
    varname[j] = '\0';
    j = 0;
    while (environ[j] != NULL)
    {
        if (ft_strncmp(environ[j], varname, ft_strlen(varname)) == 0 && environ[j][ft_strlen(varname)] == '=')
        {
            while (environ[j + 1] != NULL)
            {
                environ[j] = environ[j + 1];
                j++;
            }
            environ[j] = NULL;
            break;
        }
        j++;
    }
}
