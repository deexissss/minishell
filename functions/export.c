/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:52:23 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/13 10:59:25 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern char	**environ;
;

void	add_var(char **new_environ, int env_size, char *vname, char *value)
{
	int	j;
	int	i;

	i = 0;
	j = 0;
	new_environ[env_size] = malloc(ft_strlen(vname) + ft_strlen(value) + 2);
	if (!new_environ[env_size])
	{
		perror("malloc");
		for (int k = 0; k < env_size; k++)
			free(new_environ[k]);
		free(new_environ);
		return ;
	}
	while (vname[j] != '\0')
	{
		new_environ[env_size][j] = vname[j];
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

void	remove_var(char *varname)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(varname);
	while (g_env.variables[i] != NULL)
	{
		if (ft_strncmp(g_env.variables[i], varname, len) == 0
			&& g_env.variables[i][len] == '=')
		{
			free(g_env.variables[i]);
			break ;
		}
		i++;
	}
	if (g_env.variables[i] == NULL)
		return ;
	while (g_env.variables[i + 1] != NULL)
	{
		g_env.variables[i] = g_env.variables[i + 1];
		i++;
	}
	g_env.variables[i] = NULL;
	g_env.size--;
}

int custom_setenv(const char *name, const char *value, int overwrite)
{
    int i;
    int name_len;
    char *new_var;

    name_len = ft_strlen(name);
    for (i = 0; i < g_env.size; i++)
    {
        if (ft_strncmp(g_env.variables[i], name, name_len) == 0 && g_env.variables[i][name_len] == '=')
        {
            if (!overwrite)
                return 0;
            free(g_env.variables[i]);
            new_var = malloc(name_len + ft_strlen(value) + 2);
            if (!new_var)
            {
                perror("malloc");
                return -1;
            }
            sprintf(new_var, "%s=%s", name, value);
            g_env.variables[i] = new_var;
            return 0;
        }
    }
    new_var = malloc(name_len + ft_strlen(value) + 2);
    if (!new_var)
    {
        perror("malloc");
        return -1;
    }
    sprintf(new_var, "%s=%s", name, value);
    g_env.variables[g_env.size] = new_var;
    g_env.variables[++g_env.size] = NULL;
    return 0;
}

void	handle_new_var(char *varname, char *value)
{
	char	**new_variables;
	int		i;

	new_variables = malloc(sizeof(char *) * (g_env.size + 2));
	if (!new_variables)
	{
		perror("malloc");
		return ;
	}
	for (i = 0; i < g_env.size; i++)
	{
		new_variables[i] = g_env.variables[i];
	}
	new_variables[g_env.size] = malloc(ft_strlen(varname) + ft_strlen(value)
			+ 2);
	if (!new_variables[g_env.size])
	{
		perror("malloc");
		free(new_variables);
		return ;
	}
	sprintf(new_variables[g_env.size], "%s=%s", varname, value);
	new_variables[g_env.size + 1] = NULL;
	free(g_env.variables);
	g_env.variables = new_variables;
	g_env.size++;
	if (strcmp(varname, "PATH") == 0)
    {
        if (custom_setenv(varname, value, 1) != 0)
        {
            perror("setenv");
        }
    }
}

void	extract_var(char *input, char **vname, char **val, int *index)
{
	skip_whitespace(input, index);
	if (input[*index] == '\0')
	{
		printf("error: export missing operand\n");
		g_exit_status = 1;
		return ;
	}
	if (!extract_varname(input, vname, index))
	{
		g_exit_status = 1;
		return ;
	}
	extract_val(input, val, index);
}

void	execute_export(char *input)
{
	char *varname;
	char *value;
	int index;

	index = 6;
	g_exit_status = 0;
	while (input[index] != '\0')
	{
		extract_var(input, &varname, &value, &index);
		if (g_exit_status != 0)
		{
			free(varname);
			free(value);
			return ;
		}
		remove_var(varname);
		handle_new_var(varname, value);
		if (g_exit_status != 0)
		{
			free(varname);
			free(value);
			return ;
		}
		if (ft_strcmp(varname, "PATH") == 0)
			verify_path_order(value);
		free(varname);
		free(value);
		skip_whitespace(input, &index);
	}
}