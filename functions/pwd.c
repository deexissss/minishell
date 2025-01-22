/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 09:07:10 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/22 09:50:26 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	validate_pwd_command(char *command)
{
	int	i;

	i = 0;
	while (command[i] >= 'a' && command[i] <= 'z' && i < 3)
		i++;
	if (command[i] >= 'a' && command[i] <= 'z')
	{
		printf("error: command does not exit\n");
		g_env.exit_status = 127;
	}
	else if (command[i] != '\0')
	{
		printf("error: pwd does not take any argument\n");
		g_env.exit_status = 1;
	}
}

void	print_current_directory(void)
{
	char	cwd[4096];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
	{
		perror("getcwd");
		exit(1);
	}
}

void	execute_pwd(char *command)
{
	pid_t	pid;

	g_env.exit_status = 0;
	validate_pwd_command(command);
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
		print_current_directory();
		exit(0);
	}
	else
		waitpid(pid, NULL, 0);
}

