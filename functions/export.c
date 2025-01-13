#include "../minishell.h"

extern char **environ;;

/*void extract_varname_and_value(char *input, char *varname, char *value, int *index)
{
    int i;
    unsigned long j;

    i = *index;
    j = 0;
    while (input[i] == ' ' || input[i] == '\t')
        i++;
    if (input[i] == '\0')
    {
        printf("error: export missing operand\n");
        exit_status = 1;
        *index = i;
        return;
    }
    while (input[i] && input[i] != '=' && input[i] != ' ' && input[i] != '\t' && j < sizeof(varname) - 1)
        varname[j++] = input[i++];
    varname[j] = '\0';
    if (input[i] != '=')
    {
        printf("error: export missing '=' after variable name\n");
        *index = i;
        return;
    }
    i++;
    j = 0;
    while (input[i] && input[i] != ' ' && input[i] != '\t' && j < sizeof(value) - 1)
        value[j++] = input[i++];
    value[j] = '\0';
    *index = i;
}

int get_env_size(void)
{
    int env_size;

    env_size = 0;
    while (environ[env_size] != NULL)
        env_size++;
    return env_size;
}

void add_variable_to_environ(char **new_environ, int env_size, char *varname, char *value)
{
    int j;
    int i;

    i = 0;
    j = 0;
    new_environ[env_size] = malloc(strlen(varname) + strlen(value) + 2);
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
    while (value[i] != '\0')
        new_environ[env_size][j++] = value[i++];
    new_environ[env_size][j] = '\0';
}

void remove_existing_var(char *varname)
{
    int i;
    int j;
    int len;

    i = 0;
    len = strlen(varname);
    while (environ[i] != NULL)
    {
        if (strncmp(environ[i], varname, len) == 0 && environ[i][len] == '=')
        {
            j = i;
            while (environ[j] != NULL)
            {
                environ[j] = environ[j + 1];
                j++;
            }
            return;
        }
        i++;
    }
}

void execute_export(char *input)
{
    char varname[256];
    char value[256];
    int index ;

    index = 6;
    exit_status = 0;
    while (input[index] != '\0')
    {
        extract_varname_and_value(input, varname, value, &index);
        if (exit_status != 0)
            return;
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
        while (input[index] == ' ' || input[index] == '\t')
            index++;
    }
}*/

void skip_whitespace(char *input, int *i)
{
    while (input[*i] == ' ' || input[*i] == '\t')
        (*i)++;
}

int extract_varname(char *input, char *varname, int *i)
{
    unsigned long j;

    j = 0;
    while (input[*i] && input[*i] != '=' && input[*i] != ' ' &&
           input[*i] != '\t' && j < sizeof(varname) - 1)
        varname[j++] = input[(*i)++];
    varname[j] = '\0';
    if (input[*i] != '=')
    {
        printf("error: export missing '=' after variable name\n");
        return 0;
    }
    (*i)++;
    return (1);
}

void extract_value(char *input, char *value, int *i)
{
    unsigned long j;

    j = 0;
    while (input[*i] && input[*i] != ' ' && input[*i] != '\t' &&
           j < sizeof(value) - 1)
        value[j++] = input[(*i)++];
    value[j] = '\0';
}

int get_env_size(void)
{
    int env_size;

    env_size = 0;
    while (environ[env_size] != NULL)
        env_size++;
    return (env_size);
}

void add_variable(char **new_environ, int env_size, char *varname, char *value)
{
    int j;
    int i;

    i = 0;
    j = 0;
    new_environ[env_size] = malloc(strlen(varname) + strlen(value) + 2);
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
    while (value[i] != '\0')
    {
        new_environ[env_size][j] = value[i];
        j++;
        i++;
    }
    new_environ[env_size][j] = '\0';
}


void remove_var(char *varname)
{
    int i;
    int j;
    int len;

    i = 0;
    len = ft_strlen(varname);
    while (environ[i] != NULL)
    {
        if (strncmp(environ[i], varname, len) == 0 && environ[i][len] == '=')
        {
            j = i;
            while (environ[j] != NULL)
            {
                environ[j] = environ[j + 1];
                j++;
            }
            return;
        }
        i++;
    }
}

void handle_new_variable(char *varname, char *value)
{
    int env_size;
    char **new_environ;
    int i;

    env_size = get_env_size();
    new_environ = malloc(sizeof(char *) * (env_size + 2));
    i = 0;
    if (!new_environ)
    {
        perror("malloc");
        return;
    }
    while (i < env_size)
    {
        new_environ[i] = environ[i];
        i++;
    }
    add_variable(new_environ, env_size, varname, value);
    new_environ[env_size + 1] = NULL;
    environ = new_environ;
}

void extract_varname_and_value(char *input, char *varname, char *value, int *index)
{
    skip_whitespace(input, index);
    if (input[*index] == '\0')
    {
        printf("error: export missing operand\n");
        exit_status = 1;
        return;
    }
    if (!extract_varname(input, varname, index))
    {
        exit_status = 1;
        return;
    }
    extract_value(input, value, index);
}

void execute_export(char *input)
{
    char varname[256];
    char value[256];
    int index;

    index = 6;
    exit_status = 0;
    while (input[index] != '\0')
    {
        extract_varname_and_value(input, varname, value, &index);
        if (exit_status != 0)
            return;
        remove_var(varname);
        handle_new_variable(varname, value);
        skip_whitespace(input, &index);
    }
}
