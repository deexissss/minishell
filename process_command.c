/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:07:32 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/13 15:09:04 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_single_command(char *command)
{
	char	*clean_command;

	clean_command = cleanup_string(command);
	if (clean_command)
	{
		ft_checker(clean_command);
		free(clean_command);
	}
}

static void	handle_commands(char **commands, int num_commands)
{
	int	i;

	i = 0;
	while (i < num_commands)
	{
		if ((ftstrchr(commands[i], '>') || ftstrchr(commands[i], '<'))
			&& !is_redirection_inside_quotes(commands[i]))
			execute_redirection(commands[i]);
		else
			handle_single_command(commands[i]);
		i++;
	}
}

static void	process_piped_commands(char *command)
{
	int		num_commands;
	int		i;
	char	**commands;

	i = 0;
	commands = pipe_tokenizer(command, &num_commands);
	if (commands)
	{
		if (num_commands > 1)
			execute_pipeline(commands, num_commands);
		else
			handle_commands(commands, num_commands);
		while (i < num_commands)
			free(commands[i++]);
		free(commands);
	}
}

void	process_command(char *command)
{
	if (!command)
		return ;
	if (!is_pipe_inside_quotes(command))
	{
		process_piped_commands(command);
	}
	else if ((ftstrchr(command, '>') || ftstrchr(command, '<'))
		&& !is_redirection_inside_quotes(command))
		execute_redirection(command);
	else
		handle_single_command(command);
}
