#include "../minishell.h"

extern char **environ;;

void    extract_varname_and_value(char *inpt, char *varname, char *value)
{
    int i = 6;
    long unsigned int j = 0;
    
    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    if (inpt[i] == '\0')
    {
        printf("error: export missing operand\n");
        return;
    }
    while (inpt[i] && inpt[i] != '=' && inpt[i] != '\t' && j < sizeof(varname) - 1)
        varname[j++] = inpt[i++];
    varname[j] = '\0';
    if (inpt[i] != '=')
    {
        printf("error: export missing '=' after variable name\n");
        return;
    }
    i++;
    j = 0;
    while (inpt[i] && j < sizeof(value) - 1)
        value[j++] = inpt[i++];
    value[j] = '\0';
}

int     get_env_size(void)
{
    int env_size = 0;
    while (environ[env_size] != NULL)
        env_size++;
    return env_size;
}

void    add_variable_to_environ(char **new_environ, int env_size, char *varname, char *value)
{
    int j = 0;

    new_environ[env_size] = malloc(ft_strlen(varname) + ft_strlen(value) + 2);
    if (!new_environ[env_size])
    {
        perror("malloc");
        free(new_environ);
        return;
    }
    while (varname[j] != '\0')
    {
        new_environ[env_size][j] = varname[j];
        j++;
    }
    new_environ[env_size][j++] = '=';
    int i = 0;
    while (value[i] != '\0')
        new_environ[env_size][j++] = value[i++];
    new_environ[env_size][j] = '\0';
}

void    remove_existing_var(char *varname)
{
    int i;
    int len;

    i = 0;
    len = ft_strlen(varname);
    while (environ[i] != NULL)
    {
        if (strncmp(environ[i], varname, len) == 0 && environ[i][len] == '=')
        {
            free(environ[i]);
            while (environ[i] != NULL)
            {
                environ[i] = environ[i + 1];
                i++;
            }
            return;
        }
        i++;
    }
}

void    execute_export(char *inpt)
{
    char varname[256];
    char value[256];

    extract_varname_and_value(inpt, varname, value);
    remove_existing_var(varname);
    int env_size = get_env_size();
    char **new_environ = malloc(sizeof(char *) * (env_size + 2));
    if (!new_environ)
    {
        perror("malloc");
        return;
    }

    int k = 0;
    while (k < env_size)
    {
        new_environ[k] = environ[k];
        k++;
    }
    add_variable_to_environ(new_environ, env_size, varname, value);
    new_environ[env_size + 1] = NULL;
    environ = new_environ;
}
