/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 14:47:52 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/15 09:25:50 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_path(char *path, char **args)
{
	path = command_path(args[0]);
	if (!path)
	{
		g_exit_status = 1;
		free(args);
		return (1);
	}
	if (path == NULL)
	{
		printf("\n");
		free(args);
		return (1);
	}
	free(path);
	return (0);
}

int	check_true_command(char *path, char **args)
{
	if (check_path(path, args) == 1)
		return (1);
	else if (access(path, X_OK) != 0)
	{
		printf("error: command '%s' not found\n", args[0]);
		g_exit_status = 127;
		free(path);
		free(args);
		return (1);
	}
	else
		return (0);
}

void	free_args(char *path, char **args)
{
	int	i;

	i = 0;
	free(path);
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	exec_perror(char *str)
{
	perror(str);
	g_exit_status = 1;
}

void	handle_external_command(char *command)
{
	char	**args;
	char	*path;
	pid_t	pid;
	int		status;

	args = ft_split(command, ' ');
	if (!correct_command(args))
		return ;
	path = command_path(args[0]);
	if (check_true_command(path, args) == 1)
		return ;
	signal(SIGINT, handle_sigint);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		execute_command(path, args);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			g_exit_status = 1;
	}
	else
		exec_perror("fork");
	free_args(path, args);
}
