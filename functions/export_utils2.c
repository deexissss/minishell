/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdaman <mdaman@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:16:15 by mdaman            #+#    #+#             */
/*   Updated: 2025/01/22 13:16:17 by mdaman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_environ(char **new_environ, int env_size)
{
	int	j;

	j = 0;
	while (j < env_size)
	{
		free(new_environ[j]);
		j++;
	}
	free(new_environ);
}

void	add_var(char **new_environ, int env_size, char *vname, char *value)
{
	int	j;
	int	i;

	i = 0;
	j = 0;
	new_environ[env_size] = malloc(ft_strlen(vname) + ft_strlen(value) + 2);
	if (!new_environ[env_size])
	{
		free_environ(new_environ, env_size);
		return ;
	}
	j = 0;
	while (vname[j] != '\0')
	{
		new_environ[env_size][j] = vname[j];
		j++;
	}
	new_environ[env_size][j++] = '=';
	while (value[i] != '\0')
		new_environ[env_size][j++] = value[i++];
	new_environ[env_size][j] = '\0';
}

void	print_exported_variables(t_env *env)
{
	int	i;

	i = 0;
	while (env->variables[i] != NULL)
	{
		printf("declare -x %s\n", env->variables[i]);
		i++;
	}
}
