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

/*void	handle_external_command(char *command)
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
	{
		g_exit_status = 1;
		free(args);
		return ;
	}
	if (path == NULL)
	{
		printf("\n");
		free(args);
		return ;
	}
	else if (access(path, X_OK) != 0)
	{
		printf("error: command '%s' not found\n", args[0]);
		g_exit_status = 127;
		free(path);
		free(args);
		return ;
	}
	pid = fork();
	if (pid == 0)
		execute_command(path, args);
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			g_exit_status = 1;
		}
	}
	else
	{
		perror("fork");
		g_exit_status = 1;
	}
	free(path);
	free(args);
}*/