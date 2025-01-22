/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 09:12:15 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/22 09:50:01 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


static void	check_var(char *input, int *i)
{
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
	if (input[*i] == '\0')
	{
		printf("error: unset needs a variable name\n");
		g_env.exit_status = 1;
	}
}

static void	extract_varname_u(char *input, char *varname, int *i)
{
	unsigned long	j;

	j = 0;
	while (input[*i] && input[*i] != ' ' && input[*i] != '\t'
		&& j < sizeof(varname) - 1)
		varname[j++] = input[(*i)++];
	varname[j] = '\0';
}

static void	remove_var_from_env(char *varname)
{
    int	i;
    int varname_len;

	i = 0;
	varname_len = ft_strlen(varname);
	while (g_env.variables[i] != NULL)
	{
		if (ft_strncmp(g_env.variables[i], varname, varname_len) == 0
			&& g_env.variables[i][varname_len] == '=')
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

void	execute_unset(char *input)
{
	int		i;
	char	varname[256];

	i = 6;
	g_env.exit_status = 0;
	while (input[i] != '\0')
	{
		check_var(input, &i);
		if (g_env.exit_status != 0)
			return ;
		extract_varname_u(input, varname, &i);
		remove_var_from_env(varname);
		while (input[i] == ' ' || input[i] == '\t')
			i++;
	}
}
