/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handlers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 14:50:06 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/13 14:50:32 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Function to handle input redirection (<)
void	handle_input_redirection(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return ;
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return ;
	}
	close(fd);
}

void	handle_output_redirection(char *filename, bool append)
{
	int	fd;

	while (*filename == ' ' || *filename == '\t')
		filename++;
	if (append)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		return ;
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return ;
	}
	close(fd);
}

void	read_and_write_heredoc(const char *delimiter, int pipefd[2])
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (line == NULL)
		{
			perror("readline");
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

void	handle_heredoc_redirection(const char *delimiter)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return ;
	}
	read_and_write_heredoc(delimiter, pipefd);
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
}

void	handle_command_or_args(char *token, char **cmd, char **args)
{
	char	*temp;
	char	*new_args;

	if (*cmd == NULL)
		*cmd = ft_strdup(token);
	else
	{
		if (*args == NULL)
			*args = ft_strdup(token);
		else
		{
			temp = ft_strjoin(*args, " ");
			new_args = ft_strjoin(temp, token);
			free(temp);
			free(*args);
			*args = new_args;
		}
	}
}
