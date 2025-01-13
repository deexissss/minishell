/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 09:12:15 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/13 11:00:38 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern char	**environ;;
static void	check_var(char *input, int *i)
{
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
	if (input[*i] == '\0')
	{
		printf("error: unset needs a variable name\n");
		g_exit_status = 1;
	}
}

static void	extract_varname_u(char *input, char *varname, int *i)
{
	unsigned long	j;

	j = 0;
	while (input[*i] && input[*i] != ' ' && input[*i] != '\t'
		&&j < sizeof(varname) - 1)
		varname[j++] = input[(*i)++];
	varname[j] = '\0';
}

static void	remove_var_from_env(char *varname)
{
	unsigned long	j;

	j = 0;
	while (environ[j] != NULL)
	{
		if (ft_strncmp(environ[j], varname, strlen(varname)) == 0
			&& environ[j][strlen(varname)] == '=')
		{
			while (environ[j + 1] != NULL)
			{
				environ[j] = environ[j + 1];
				j++;
			}
			environ[j] = NULL;
			return ;
		}
		j++;
	}
}

void	execute_unset(char *input)
{
	int		i;
	char	varname[256];

	i = 6;
	g_exit_status = 0;
	while (input[i] != '\0')
	{
		check_var(input, &i);
		if (g_exit_status != 0)
			return ;
		extract_varname_u(input, varname, &i);
		remove_var_from_env(varname);
		while (input[i] == ' ' || input[i] == '\t')
			i++;
	}
}
