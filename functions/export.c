/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:52:23 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/22 15:29:55 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	skip_whitespace(char *input, int *i)
{
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
}

void	remove_var(t_env *env, char *varname)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(varname);
	while (env->variables[i] != NULL)
	{
		if (ft_strncmp(env->variables[i], varname, len) == 0
			&& env->variables[i][len] == '=')
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

void	extract_var(t_env *env, char *input, char **vars[], int *index)
{
	skip_whitespace(input, index);
	if (input[*index] == '\0')
	{
		printf("error: export missing operand\n");
		env->exit_status = 1;
		return ;
	}
	if (!extract_varname(input, vars[0], index))
	{
		env->exit_status = 1;
		free(*vars[0]);
		return ;
	}
	extract_val(input, vars[1], index);
}

void	handle_var(t_env *env, char *input, int *index)
{
	char	*varname;
	char	*value;
	char	**vars[2];

	vars[0] = &varname;
	vars[1] = &value;
	extract_var(env, input, vars, index);
	if (env->exit_status != 0)
		return ;
	remove_var(env, varname);
	handle_new_var(env, varname, value);
	if (env->exit_status != 0)
		return ;
	if (ft_strcmp(varname, "PATH") == 0)
		verify_path_order(value);
	free(varname);
	free(value);
}

void	execute_export(t_env *env, char *input)
{
	int	index;

	index = 6;
	env->exit_status = 0;
	while (input[index] != '\0')
	{
		handle_var(env, input, &index);
		if (env->exit_status != 0)
			return ;
		skip_whitespace(input, &index);
	}
}
