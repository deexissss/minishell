#include "minishell.h"

char	*cleanup_string(char *str)
{
	int		i;
	int		j;
	char	*clean_str;
	int		find_quote;
	char	quote;

	i = 0;
	j = 0;
	find_quote = 0;
	clean_str = malloc(strlen(str) + 1);
	if (!clean_str)
		return (NULL);
	if (memcmp(str, "echo", 4) == 0)
	{
		execute_echo(str);
		return (NULL);
	}
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

char	*handle_dollar(char *command)
{
	char *result;
	char *var_start;
	char *var_end;
	char *var_value;
	int i;
	int j;
	int in_single_quote = 0;

	result = malloc((sizeof(char) * (strlen(command) + 1) * 2));
	if (!result)
	{
		perror("malloc");
		return (NULL);
	}

	i = 0;
	j = 0;

	while (command[i] != '\0')
	{
		if (command[i] == '\'')
		{
			in_single_quote = !in_single_quote;
			result[j++] = command[i++];
		}
		else if (command[i] == '$' && command[i + 1] == '?')
		{
			char g_exit_statusc[12];
			// Assez grand pour contenir un entier
			sprintf(g_exit_statusc, "%d", g_exit_status);
			// Remplacez g_exit_status_value par la variable contenant le statut de sortie
			strcpy(&result[j], g_exit_statusc);
			j += strlen(g_exit_statusc);
			i += 2;
		}
		else if (command[i] == '$' && (i == 0 || command[i - 1] != '\\')
			&& !in_single_quote)
		{
			var_start = &command[i + 1];
			var_end = var_start;
			while (*var_end && (isalnum((int)*var_end) || *var_end == '_'))
				var_end++;
			char var_name[var_end - var_start + 1];
			strncpy(var_name, var_start, var_end - var_start);
			var_name[var_end - var_start] = '\0';
			var_value = get_env_value(var_name);
			if (var_value)
			{
				strcpy(&result[j], var_value);
				j += strlen(var_value);
				free(var_value);
			}
			i = var_end - command;
		}
		else
			result[j++] = command[i++];
	}
	result[j] = '\0';
	return (result);
}