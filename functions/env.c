/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:45:19 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/22 10:15:30 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_environment_variables(void)
{
	int	i;

	i = 0;
	while (g_env.variables[i] != NULL)
	{
		printf("%s\n", g_env.variables[i]);
		i++;
	}
}

void	validate_env_command(char *command)
{
	int	i;

	i = 0;
	while (command[i] >= 'a' && command[i] <= 'z' && i < 3)
		i++;
	if (command[i] >= 'a' && command[i] <= 'z')
	{
		printf("error: command does not exist\n");
		g_env.exit_status = 127;
		return ;
	}
	else if (command[i] != '\0')
	{
		printf("error: env dont take any argument\n");
		g_env.exit_status = 1;
		return ;
	}
}

void	execute_env(char *command)
{
	pid_t	pid;

	g_env.exit_status = 0;
	validate_env_command(command);
	if (g_env.exit_status != 0)
		return ;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	else if (pid == 0)
	{
		print_environment_variables();
		exit(0);
	}
	else
	{
		if (waitpid(pid, NULL, 0) == -1)
			perror("waitpid");
	}
}