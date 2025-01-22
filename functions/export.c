/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:52:23 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/22 10:32:42 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	skip_whitespace(char *input, int *i)
{
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
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

void	extract_var(char *input, char **vname, char **val, int *index)
{
	skip_whitespace(input, index);
	if (input[*index] == '\0')
	{
		printf("error: export missing operand\n");
		g_env.exit_status = 1;
		return ;
	}
	if (!extract_varname(input, vname, index))
	{
		g_env.exit_status = 1;
		return ;
	}
	extract_val(input, val, index);
}

void	handle_var(char *input, int *index)
{
	char	*varname;
	char	*value;

	extract_var(input, &varname, &value, index);
	if (g_env.exit_status != 0)
	{
		free(varname);
		free(value);
		return ;
	}
	remove_var(varname);
	handle_new_var(varname, value);
	if (g_env.exit_status != 0)
	{
		free(varname);
		free(value);
		return ;
	}
	if (ft_strcmp(varname, "PATH") == 0)
		verify_path_order(value);
	free(varname);
	free(value);
}

void	execute_export(char *input)
{
	int	index;

	index = 6;
	g_env.exit_status = 0;
	while (input[index] != '\0')
	{
		handle_var(input, &index);
		if (g_env.exit_status != 0)
			return ;
		skip_whitespace(input, &index);
	}
}
