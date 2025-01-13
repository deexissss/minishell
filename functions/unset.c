#include "../minishell.h"

extern char **environ;;

/*static void check_var(char *input, int *i)
{
    while (input[*i] == ' ' || input[*i] == '\t')
        (*i)++;
    if (input[*i] == '\0')
    {
        printf("error: unset needs a variable name\n");
        exit_status = 1;
        return;
    }
}
void execute_unset(char *inpt)
{
    int i;
    char varname[256];
    long unsigned int j;

    i = 6;
    exit_status = 0;
    while (inpt[i] != '\0')
    {
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
        while (inpt[i] == ' ' || inpt[i] == '\t')
            i++;
    }
}*/

static void check_var(char *input, int *i)
{
    while (input[*i] == ' ' || input[*i] == '\t')
        (*i)++;
    if (input[*i] == '\0')
    {
        printf("error: unset needs a variable name\n");
        exit_status = 1;
    }
}

static void extract_varname(char *input, char *varname, int *i)
{
    unsigned long j;

    j = 0;
    while (input[*i] && input[*i] != ' ' && input[*i] != '\t' &&
           j < sizeof(varname) - 1)
    {
        varname[j++] = input[(*i)++];
    }
    varname[j] = '\0';
}

static void remove_var_from_env(char *varname)
{
    unsigned long j;

    j = 0;
    while (environ[j] != NULL)
    {
        if (strncmp(environ[j], varname, strlen(varname)) == 0 &&
            environ[j][strlen(varname)] == '=')
        {
            while (environ[j + 1] != NULL)
            {
                environ[j] = environ[j + 1];
                j++;
            }
            environ[j] = NULL;
            return;
        }
        j++;
    }
}

void execute_unset(char *input)
{
    int i;
    char varname[256];

    i = 6;
    exit_status = 0;
    while (input[i] != '\0')
    {
        check_var(input, &i);
        if (exit_status != 0)
            return;
        extract_varname(input, varname, &i);
        remove_var_from_env(varname);
        while (input[i] == ' ' || input[i] == '\t')
            i++;
    }
}
