/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_fonctions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 11:28:38 by tjehaes           #+#    #+#             */
/*   Updated: 2025/02/12 11:31:28 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_env(t_env *env)
{
	int	i;

	i = 0;
	while (env->variables[i] != NULL)
	{
		printf("%s\n", env->variables[i]);
		i++;
	}
}

void	pipe_export(t_env *env)
{
	int	i;

	i = 0;
	while (env->variables[i] != NULL)
	{
		printf("declare -x %s\n", env->variables[i]);
		i++;
	}
}

void	pipe_pwd(void)
{
	char	cwd[4096];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("getcwd");
}

int	check_builtin(t_env *env, char *command)
{
	if (ft_memcmp(command, "pwd", 3) == 0)
	{
		pipe_pwd();
		return (1);
	}
	else if (ft_memcmp(command, "env", 3) == 0)
	{
		pipe_env(env);
		return (1);
	}
	else if (ft_memcmp(command, "export", 6) == 0)
	{
		pipe_export(env);
		return (1);
	}
	else if (ft_memcmp(command, "cd ", 3) == 0)
	{
		env->exit_status = 1;
		return (1);
	}
	return (0);
}
