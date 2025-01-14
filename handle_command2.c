/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 14:47:52 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/13 14:48:59 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_command_error(char **args)
{
	g_exit_status = 1;
	free(args);
	return ;
}

void	handle_access_error(char *path, char **args)
{
	printf("error: command '%s' not found\n", args[0]);
	g_exit_status = 127;
	free(path);
	free(args);
	return ;
}

void	wait_for_child(pid_t pid, int *status)
{
	waitpid(pid, status, 0);
	if (WIFEXITED(*status) && WEXITSTATUS(*status) != 0)
		g_exit_status = 1;
}

void	handle_fork_error(void)
{
	perror("fork");
	g_exit_status = 1;
}

void	handle_external_command(char *command)
{
	char	**args;
	char	*path;
	pid_t	pid;
	int		status;

	g_exit_status = 0;
	args = ft_split(command, ' ');
	if (!correct_command(args))
		return ;
	path = command_path(args[0]);
	if (!path)
		handle_command_error(args);
	if (access(path, X_OK) != 0)
		handle_access_error(path, args);
	pid = fork();
	if (pid == 0)
		execute_command(path, args);
	else if (pid > 0)
		wait_for_child(pid, &status);
	else
		handle_fork_error();
	free(path);
	free(args);
}
