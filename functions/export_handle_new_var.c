#include "../minishell.h"

char	**allocate_new_variables(int size)
{
	char	**new_variables;

	new_variables = malloc(sizeof(char *) * (size + 2));
	if (!new_variables)
		perror("malloc");
	return (new_variables);
}
char	*create_variable_string(const char *varname, const char *value)
{
	char	*new_var;
	int		varname_len;
	int		value_len;
	int		total_len;

	varname_len = ft_strlen(varname);
	value_len = ft_strlen(value);
	total_len = varname_len + value_len + 2;
	new_var = malloc(total_len);
	if (!new_var)
	{
		perror("malloc");
		return (NULL);
	}
	ft_memcpy(new_var, varname, varname_len);
	new_var[varname_len] = '=';
	ft_memcpy(new_var + varname_len + 1, value, value_len);
	new_var[total_len - 1] = '\0';
	return (new_var);
}

void	handle_new_var(char *varname, char *value)
{
	char **new_variables;
	int i;

	new_variables = allocate_new_variables(g_env.size);
	if (!new_variables)
		return ;
	i = 0;
	while (i < g_env.size)
	{
		new_variables[i] = g_env.variables[i];
		i++;
	}
	new_variables[g_env.size] = create_variable_string(varname, value);
	if (!new_variables[g_env.size])
	{
		free(new_variables);
		return ;
	}
	new_variables[g_env.size + 1] = NULL;
	free(g_env.variables);
	g_env.variables = new_variables;
	g_env.size++;
	if (strcmp(varname, "PATH") == 0 && custom_setenv(varname, value, 1) != 0)
		perror("setenv");
}