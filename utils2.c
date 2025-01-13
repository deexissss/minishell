#include "minishell.h"

char	*clean_string(char *str, int i, int j, char *clean_str)
{
	char	quote;
	int		find_quote;

	find_quote = 0;
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

char	*cleanup_string(char *str)
{
	char	*clean_str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	clean_str = malloc(strlen(str) + 1);
	if (!clean_str)
		return (NULL);
	if (memcmp(str, "echo", 4) == 0)
	{
		execute_echo(str);
		return (NULL);
	}
	clean_str = clean_string(str, i, j, clean_str);
	return (clean_str);
}
