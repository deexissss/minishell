/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_heredoc.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:25:20 by mdaman            #+#    #+#             */
/*   Updated: 2025/02/07 14:47:59 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	g_sigint_received = 1;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_redisplay();
	exit(130);
}

void	handle_heredoc_input(const char *delimiter, int pipefd[2], t_env *env)
{
	char	*line;

	while (1)
	{
		if (g_sigint_received)
		{
			env->exit_status = 130;
			break ;
		}
		line = readline("> ");
		if (line == NULL)
		{
			printf("bash: error here-document delimited by ");
			printf("end-of-file (wanted `%s')\n", delimiter);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
}

void	read_and_write_heredoc(const char *delimiter, int pipefd[2], t_env *env)
{
	signal(SIGINT, handle_sigint_heredoc);
	handle_heredoc_input(delimiter, pipefd, env);
	close(pipefd[1]);
	exit(env->exit_status);
}
/*void	read_and_write_heredoc(const char *delimiter, int pipefd[2], t_env *env)
{
	char	*line;
	int		status;

	signal(SIGINT, handle_sigint_heredoc);
	while (1)
	{
		if (g_sigint_received)
		{
			env->exit_status = 130;
			break ;
		}
		line = readline("> ");
		if (line == NULL)
		{
			printf("bash: error");
			printf("here-document delimited by end-of-file");
			printf("(wanted `%s')\n", delimiter);
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	exit(env->exit_status);
}*/

void	parent_process_heredoc(int pipefd[2], pid_t pid, t_env *env)
{
	int	status;

	close(pipefd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		env->exit_status = WEXITSTATUS(status);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
}

void	handle_heredoc_redirection(const char *delimiter, t_env *env)
{
	int		pipefd[2];
	pid_t	pid;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
		child_process_heredoc(delimiter, pipefd, env);
	else
		parent_process_heredoc(pipefd, pid, env);
	signal(SIGINT, handle_sigint_heredoc);
	signal(SIGQUIT, SIG_DFL);
}
