/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_heredoc.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:25:20 by mdaman            #+#    #+#             */
/*   Updated: 2025/02/12 08:41:37 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

void	read_and_write_heredoc(const char *delimiter, int pipefd[2])
{
	char	*line;

	signal(SIGINT, handle_sigint_heredoc);
	while (1)
	{
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
	exit(0);
}

void	child_process_heredoc(const char *delimiter, int pipefd[2])
{
	close(pipefd[0]);
	read_and_write_heredoc(delimiter, pipefd);
}

void	parent_process_heredoc(int pipefd[2], pid_t pid)
{
	int	status;

	close(pipefd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 130)
			g_signal_value = 1;
	}
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
}

void	handle_heredoc_redirection(const char *delimiter)
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
		child_process_heredoc(delimiter, pipefd);
	else
		parent_process_heredoc(pipefd, pid);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}
