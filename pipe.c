/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:09:45 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/22 15:50:58 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*bool	is_pipe_inside_quotes(const char *str)
{
	bool	in_single_quote;
	bool	in_double_quote;

	in_single_quote = false;
	in_double_quote = false;
	while (*str)
	{
		if (*str == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*str == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (*str == '|' && !in_single_quote && !in_double_quote)
			return (false);
		str++;
	}
	return (true);
}


void	execute_pipe_command(t_env *env, char *command)
{
	if ((ftstrchr(command, '>') || ftstrchr(command, '<'))
		&& !is_redirection_inside_quotes(command))
		execute_redirection(env, command);
	else
		ft_checker(env, command);
	exit(EXIT_SUCCESS);
}

void	handle_parent_process(pid_t pid, int pipefd[2], int *fd_in)
{
	waitpid(pid, NULL, 0);
	close(pipefd[1]);
	*fd_in = pipefd[0];
}

void	execute_pipeline(t_env *env, char **commands, int num_commands)
{
	int		pipefd[2];
	pid_t	pid;
	int		fd_in;
	int		i;

	fd_in = 0;
	i = 0;
	while (i < num_commands)
	{
		pipe_check(pipefd);
		pid = fork();
		pid_check(pid);
		if (pid == 0)
		{
			dup2(fd_in, 0);
			if (i < num_commands - 1)
				dup2(pipefd[1], 1);
			close(pipefd[0]);
			close(pipefd[1]);
			execute_pipe_command(env, commands[i]);
		}
		else
			handle_parent_process(pid, pipefd, &fd_in);
		i++;
	}
}*/

bool	is_pipe_inside_quotes(const char *str)
{
	bool	in_single_quote;
	bool	in_double_quote;

	in_single_quote = false;
	in_double_quote = false;
	while (*str)
	{
		if (*str == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*str == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (*str == '|' && !in_single_quote && !in_double_quote)
			return (false);
		str++;
	}
	return (true);
}

/*void	execute_pipe_command(t_env *env, char *command)
{
	if ((ftstrchr(command, '>') || ftstrchr(command, '<'))
		&& !is_redirection_inside_quotes(command))
		execute_redirection(env, command);
	else
		ft_checker(env, command);
	exit(EXIT_SUCCESS);
}*/
void	handle_child_process(t_pipe_info *pipe_info, t_env *env, char *command)
{
	dup2(pipe_info->fd_in, 0);
	if (!pipe_info->is_last_command)
		dup2(pipe_info->pipefd[1], 1);
	close(pipe_info->pipefd[0]);
	close(pipe_info->pipefd[1]);
	if ((ftstrchr(command, '>') || ftstrchr(command, '<'))
		&& !is_redirection_inside_quotes(command))
		execute_redirection(env, command);
	else
		ft_checker(env, command);
	exit(EXIT_SUCCESS);
}

void	handle_parent_process(int pipefd[2], int *fd_in)
{
	close(pipefd[1]);
	if (*fd_in != 0)
		close(*fd_in);
	*fd_in = pipefd[0];
}

void	execute_pipeline(t_env *env, char **commands, int num_commands)
{
	int			i;
	t_pipe_info	pipe_info;

	pipe_info.fd_in = 0;
	i = 0;
	while (i < num_commands)
	{
		pipe_check(pipe_info.pipefd);
		pipe_info.pid = fork();
		pid_check(pipe_info.pid);
		pipe_info.is_last_command = (i == num_commands - 1);
		if (pipe_info.pid == 0)
			handle_child_process(&pipe_info, env, commands[i]);
		else
			handle_parent_process(pipe_info.pipefd, &pipe_info.fd_in);
		i++;
	}
	while (wait(&pipe_info.status) > 0)
		;
}
