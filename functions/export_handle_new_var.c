/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_handle_new_var.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:17:24 by mdaman            #+#    #+#             */
/*   Updated: 2025/01/22 15:29:51 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	handle_new_var(t_env *env, char *varname, char *value)
{
	char	**new_variables;
	int		i;

	new_variables = allocate_new_variables(env->size);
	if (!new_variables)
		return ;
	i = 0;
	while (i < env->size)
	{
		new_variables[i] = env->variables[i];
		i++;
	}
	new_variables[env->size] = create_variable_string(varname, value);
	if (!new_variables[env->size])
	{
		free(new_variables);
		return ;
	}
	new_variables[env->size + 1] = NULL;
	free(env->variables);
	env->variables = new_variables;
	env->size++;
	if (strcmp(varname, "PATH") == 0 && custom_setenv(env, varname, value,
			1) != 0)
		perror("setenv");
}
