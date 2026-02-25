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
