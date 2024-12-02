#include "../minishell.h"

extern char **environ;;

void execute_unset(char *inpt)
{
    int i = 6; // Start after the "unset" command
    char varname[256];
    long unsigned int j = 0;

    // Skip leading spaces and tabs
    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;

    // Check if there is a variable name provided
    if (inpt[i] == '\0')
    {
        ft_printf("error: unset needs a variable name\n");
        return;
    }

    // Extract the variable name
    while (inpt[i] && inpt[i] != ' ' && inpt[i] != '\t' && j < sizeof(varname) - 1)
        varname[j++] = inpt[i++];
    varname[j] = '\0';

    // Remove the variable from the environment
    j = 0;
    while (environ[j] != NULL)
    {
        if (strncmp(environ[j], varname, strlen(varname)) == 0 && environ[j][strlen(varname)] == '=')
        {
            // Shift all subsequent environment variables one position to the left
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
