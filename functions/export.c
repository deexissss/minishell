#include "../minishell.h"

extern char **environ;;

void    execute_export(char *inpt)
{
    int                 i;
    char                varname[256];
    char                value[256];
    long unsigned int   j;
    int                 env_size;
    char                **new_environ;
    int                 k;//to copy the environ array into new_environ

    i = 6;
    j = 0;
    env_size = 0;
    k = 0;
    ///whitespace
    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    //input after command
    if (inpt[i] == '\0')
    {
        ft_printf("error: export missing operand\n");
        return;
    }
    //extracting the variable name UNTIL = found
    while (inpt[i] && inpt[i] != '=' && inpt[i] != '\t' && j < sizeof(varname) - 1)
        varname[j++] = inpt[i++];
    varname[j] = '\0';
    //ENSURES THAT '=' character is actually present after the variable name
    if (inpt[i] != '=')
    {
        ft_printf("error: export missing '=' after variable name\n");
        return;
    }
    i++;
    //now we extract the value name after =
    j = 0;
    while (inpt[i] && j < sizeof(value) - 1)
        value[j++] = inpt[i++];
    value[j] = '\0';
    //from here we need to work on the setenv replacement
    // Get the size of the environment variables
    while (environ[env_size] != NULL)
        env_size++;
    //allocate memory for new env var
    new_environ = malloc(sizeof(char *) * (env_size + 2));
    if (!new_environ)
    {
        perror("malloc");
        return;
    }
    //copy the existing environment variables to new_environ
    while(k < env_size)
    {
        new_environ[k] = environ[k];
        k++;
    }
    // Allocate memory and set the new variable
    new_environ[env_size] = malloc(ft_strlen(varname) + ft_strlen(value) + 2);//+2 for = and \0
    if (!new_environ[env_size])
    {
        perror("malloc");
        free(new_environ);
        return;
    }
    //concatenate the variable name and value into new variable(replaces sprintf)
    j = 0;
    while (varname[j] != '\0')
    {
        new_environ[env_size][j] = varname[j];
        j++;
    }
    new_environ[env_size][j++] = '=';
    i = 0;
    while (value[i] != '\0')
    {
        new_environ[env_size][j++] = value[i++];
    }
    new_environ[env_size][j] = '\0';
    // Set the last element of the new_environ array to NULL
    new_environ[env_size + 1] = NULL;
    // Update the environment variables
    environ = new_environ;
    //do we need to free the old environ?
}