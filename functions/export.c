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

extern char	**environ;;
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
	int	j;
	int	len;

	i = 0;
	len = ft_strlen(varname);
	while (environ[i] != NULL)
	{
		if (ft_strncmp(environ[i], varname, len) == 0 && environ[i][len] == '=')
		{
			free(environ[i]);
			j = i;
			while (environ[j] != NULL)
			{
				environ[j] = environ[j + 1];
				j++;
			}
			return ;
		}
		i++;
	}
}

void	handle_new_var(char *varname, char *value)
{
	int		env_size;
	char	**new_environ;
	int		i;

	env_size = get_env_size();
	new_environ = malloc(sizeof(char *) * (env_size + 2));
	i = 0;
	if (!new_environ)
	{
		perror("malloc");
		return ;
	}
	while (i < env_size)
	{
		new_environ[i] = environ[i];
		i++;
	}
	add_var(new_environ, env_size, varname, value);
	new_environ[env_size + 1] = NULL;
	environ = new_environ;
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
	char	*varname;
	char	*value;
	int		index;

	index = 6;
	g_exit_status = 0;
	while (input[index] != '\0')
	{
		extract_var(input, &varname, &value, &index);
		if (g_exit_status != 0)
			return ;
		remove_var(varname);
		handle_new_var(varname, value);
		if (ft_strcmp(varname, "PATH") == 0)
			verify_path_order(value);
		skip_whitespace(input, &index);
	}
}
