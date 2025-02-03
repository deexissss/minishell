/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:45:19 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/22 15:10:48 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_environment_variables(t_env *env)
{
	int	i;

	i = 0;
	while (env->variables[i] != NULL)
	{
		printf("%s\n", env->variables[i]);
		i++;
	}
}

void	validate_env_command(t_env *env, char *command)
{
	int	i;

	i = 0;
	while (command[i] >= 'a' && command[i] <= 'z' && i < 3)
		i++;
	if (command[i] >= 'a' && command[i] <= 'z')
	{
		printf("error: command does not exist\n");
		env->exit_status = 127;
		return ;
	}
	else if (command[i] != '\0')
	{
		printf("error: env dont take any argument\n");
		env->exit_status = 1;
		return ;
	}
}

void	execute_env(t_env *env, char *command)
{
	pid_t	pid;

	env->exit_status = 0;
	validate_env_command(env, command);
	if (env->exit_status != 0)
		return ;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	else if (pid == 0)
	{
		print_environment_variables(env);
		exit(0);
	}
	else
	{
		if (waitpid(pid, NULL, 0) == -1)
			perror("waitpid");
	}
}
