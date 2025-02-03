/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 09:12:15 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/22 15:18:51 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	check_var(t_env *env, char *input, int *i)
{
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
	if (input[*i] == '\0')
	{
		printf("error: unset needs a variable name\n");
		env->exit_status = 1;
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

static void	remove_var_from_env(t_env *env, char *varname)
{
	int	i;
	int	varname_len;

	i = 0;
	varname_len = ft_strlen(varname);
	while (env->variables[i] != NULL)
	{
		if (ft_strncmp(env->variables[i], varname, varname_len) == 0
			&& env->variables[i][varname_len] == '=')
		{
			free(env->variables[i]);
			break ;
		}
		i++;
	}
	if (env->variables[i] == NULL)
		return ;
	while (env->variables[i + 1] != NULL)
	{
		env->variables[i] = env->variables[i + 1];
		i++;
	}
	env->variables[i] = NULL;
	env->size--;
}

void	execute_unset(t_env *env, char *input)
{
	int		i;
	char	varname[256];

	i = 6;
	env->exit_status = 0;
	while (input[i] != '\0')
	{
		check_var(env, input, &i);
		if (env->exit_status != 0)
			return ;
		extract_varname_u(input, varname, &i);
		remove_var_from_env(env, varname);
		while (input[i] == ' ' || input[i] == '\t')
			i++;
	}
}
