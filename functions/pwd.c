/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 09:07:10 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/22 15:50:55 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	validate_pwd_command(t_env *env, char *command)
{
	int	i;

	i = 0;
	while (command[i] >= 'a' && command[i] <= 'z' && i < 3)
		i++;
	if (command[i] >= 'a' && command[i] <= 'z')
	{
		printf("error: command does not exit\n");
		env->exit_status = 127;
	}
	else if (command[i] != '\0')
	{
		printf("error: pwd does not take any argument\n");
		env->exit_status = 1;
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

void	execute_pwd(t_env *env, char *command)
{
	pid_t	pid;

	env->exit_status = 0;
	validate_pwd_command(env, command);
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
		print_current_directory();
		exit(0);
	}
	else
		waitpid(pid, NULL, 0);
}
