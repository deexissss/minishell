#include "../minishell.h"

void	copy_name_value(char *new_var, const char *name, const char *value,
		int name_len)
{
	ft_memcpy(new_var, name, name_len);
	new_var[name_len] = '=';
	ft_memcpy(new_var + name_len + 1, value, strlen(value));
	new_var[name_len + strlen(value) + 1] = '\0';
}

char	*create_new_var(const char *name, const char *value)
{
	char	*new_var;
	int		name_len;
	int		value_len;

	name_len = ft_strlen(name);
	value_len = ft_strlen(value);
	new_var = malloc(name_len + value_len + 2);
	if (!new_var)
	{
		perror("malloc");
		return (NULL);
	}
	ft_memcpy(new_var, name, name_len);
	new_var[name_len] = '=';
	ft_memcpy(new_var + name_len + 1, value, value_len);
	new_var[name_len + value_len + 1] = '\0';
	return (new_var);
}

int	update_existing_var(int index, const char *name, const char *value,
		int overwrite)
{
	char	*new_var;

	if (!overwrite)
		return (0);
	free(g_env.variables[index]);
	new_var = create_new_var(name, value);
	if (!new_var)
		return (-1);
	g_env.variables[index] = new_var;
	return (0);
}

int	custom_setenv(const char *name, const char *value, int overwrite)
{
	int		i;
	int		name_len;
	char	*new_var;

	name_len = ft_strlen(name);
	i = 0;
	while (i < g_env.size)
	{
		if (ft_strncmp(g_env.variables[i], name, name_len) == 0
			&& g_env.variables[i][name_len] == '=')
			return (update_existing_var(i, name, value, overwrite));
		i++;
	}
	new_var = create_new_var(name, value);
	if (!new_var)
	{
		perror("malloc");
		return (-1);
	}
	copy_name_value(new_var, name, value, name_len);
	g_env.variables[g_env.size] = new_var;
	g_env.variables[++g_env.size] = NULL;
	return (0);
}
