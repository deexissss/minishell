#include "minishell.h"

char *get_env_value(const char *var)
{
    char *value = getenv(var);
    if (value)
        return ft_strdup(value);
    else
        return NULL;
}

char *cleanup_string(char *str)
{
    int     i;
    int     j;
    char    *clean_str;
    int     find_quote;
    char    quote;

    i = 0;
    j = 0;
    find_quote = 0;
    clean_str = malloc(strlen(str) + 1);
    if (!clean_str)
        return NULL;
    while (str[i])
    {
        if (find_quote == 0 && (str[i] == '\'' || str[i] == '"'))
        {
            find_quote = 1;
            quote = str[i];
        }
        else if (find_quote == 1 && str[i] == quote)
        {
            find_quote = 0;
            quote = '\0';
        }
        else
        {
            clean_str[j] = str[i];
            j++;
        }
        i++;
    }
    clean_str[j] = '\0';
    return (clean_str);
}

void handle_sigint(int sig)
{
    (void)sig;
    printf("\n");
    printf(BLUE "-> Minishell %s " RESET, execute_pwdmain());
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();

}

char *handle_dollar(char *command)
{
    char    *result;
    char    *var_start;
    char    *var_end;
    char    *var_value;
    int     i;
    int     j;

    result = malloc(ft_strlen(command) + 1);
    i = 0;
    j = 0;

    while(command[i] != '\0')
    {
        //check for $ and if it is not escaped
        if (command[i] == '$' && (i == 0 || command[i - 1] != '\\'))
        {
            var_start = &command[i +1];//points to the character after the $
            var_end = var_start;//takes the same position as var_start
            //traverses the variable name using the position of var_end
            while (*var_end && (ft_isalnum((int)*var_end) || *var_end == '_'))
                var_end++;
            char var_name[var_end - var_start + 1];
            ft_strlcpy(var_name, var_start, var_end - var_start); //copy temporarily the variable name
            var_name[var_end - var_start] = '\0';
            var_value = get_env_value(var_name);
            if (var_value)
            {
                ft_strcpy(&result[j], var_value);
                j += ft_strlen(var_value);
                free(var_value);
            }
            i = var_end - command;
        }
        // if the character is not a $ or the $ is escaped simply copy the character
        else
        result[j++] = command[i++];
    }
    result[j] = '\0';
    return (result);
}