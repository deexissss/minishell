/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 14:47:52 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/22 15:50:44 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_path(t_env *env, char *path, char **args)
{
	path = command_path(env, args[0]);
	if (!path)
	{
		env->exit_status = 1;
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

int	check_true_command(t_env *env, char *path, char **args)
{
	if (check_path(env, path, args) == 1)
		return (1);
	else if (access(path, X_OK) != 0)
	{
		printf("error: command '%s' not found\n", args[0]);
		env->exit_status = 127;
		return (1);
	}
	else
		return (0);
}

void	free_args(char *path, char **args)
{
	int	i;

	if (path == NULL && args == NULL)
		return ;
	else if (path)
	{
		free(path);
		path = NULL;
	}
	if (args)
	{
		i = 0;
		while (args[i])
		{
			free(args[i]);
			args[i] = NULL;
			i++;
		}
		free(args);
		args = NULL;
	}
}

void	execute_command_with_fork(t_env *env, char *path, char **args)
{
	pid_t	pid;
	int		status;

	signal(SIGINT, handle_sigint);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		execute_command(env, path, args);
		exit(env->exit_status);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		update_exit_status(env, status);
	}
	else
		exec_perror(env, "fork");
}

void	handle_external_command(t_env *env, char *command)
{
	char	**args;
	char	*path;

	args = ft_split(command, ' ');
	if (!correct_command(env, args))
	{
		free_args(NULL, args);
		return ;
	}
	path = command_path(env, args[0]);
	if (!path || check_true_command(env, path, args) == 1)
	{
		free(path);
		free_args(NULL, args);
		return ;
	}
	execute_command_with_fork(env, path, args);
	free(path);
	free_args(NULL, args);
}
