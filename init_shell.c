/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:24:49 by mdaman            #+#    #+#             */
/*   Updated: 2025/01/22 16:54:01 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_struct(t_env *env)
{
	int			i;
	extern char	**environ;

	i = 0;
	env->size = 0;
	env->exit_status = 0;
	while (environ[env->size] != NULL)
		env->size++;
	env->variables = malloc(sizeof(char *) * (env->size + 1));
	if (!env->variables)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	while (i < env->size)
	{
		env->variables[i] = ft_strdup(environ[i]);
		i++;
	}
	env->variables[i] = NULL;
}

void	free_struct(t_env *env)
{
	int	i;

	i = 0;
	while (env->variables[i] != NULL)
	{
		free(env->variables[i]);
		i++;
	}
	free(env->variables);
}

void	init_shell(t_env *env, int *saved_stdin, int *saved_stdout)
{
	init_struct(env);
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	rl_bind_key('\t', handle_tab);
	rl_bind_key(127, handle_backspace);
	using_history();
}
