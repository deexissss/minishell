/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:10:50 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/22 15:22:53 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_redirection_inside_quotes(const char *str)
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
		else if (*str == '<' && !in_single_quote && !in_double_quote)
			return (false);
		else if (*str == '>' && !in_single_quote && !in_double_quote)
			return (false);
		str++;
	}
	return (true);
}

void	handle_token(char *token, char **cmd, char **args)
{
	bool	append;
	bool	is_heredoc;

	if (ft_strcmp(token, ">") == 0 || ft_strcmp(token, ">>") == 0)
	{
		append = ft_strcmp(token, ">>") == 0;
		token = ft_strtok(NULL, " ");
		if (token != NULL)
			handle_output_redirection(token, append);
	}
	else if (ft_strcmp(token, "<") == 0 || ft_strcmp(token, "<<") == 0)
	{
		is_heredoc = ft_strcmp(token, "<<") == 0;
		token = ft_strtok(NULL, " ");
		if (token != NULL)
		{
			if (is_heredoc)
				handle_heredoc_redirection(token);
			else
				handle_input_redirection(token);
		}
	}
	else
		handle_command_or_args(token, cmd, args);
}

void	execute_command_with_redirection(t_env *env, char *cmd, char *args)
{
	char	*full_command;
	size_t	full_command_length;
	int		args_length;

	args_length = 0;
	if (cmd != NULL)
	{
		if (args)
			args_length = ft_strlen(args);
		full_command_length = ft_strlen(cmd) + args_length + 2;
		full_command = malloc(full_command_length);
		ft_strcpy(full_command, cmd);
		if (args)
		{
			ft_strlcat(full_command, " ", full_command_length);
			ft_strlcat(full_command, args, full_command_length);
		}
		ft_checker(env, full_command);
		free(full_command);
		free(cmd);
		if (args)
			free(args);
	}
}

void	execute_parsed_command(t_env *env, char *cmd, char *args)
{
	int		num_commands;
	int		i;
	char	**commands;

	i = 0;
	if (cmd != NULL)
	{
		if (ftstrchr(cmd, '|') && !is_pipe_inside_quotes(cmd))
		{
			commands = pipe_tokenizer(cmd, &num_commands);
			if (commands)
			{
				execute_pipeline(env, commands, num_commands);
				while (i < num_commands)
					free(commands[i++]);
				free(commands);
			}
		}
		else
			execute_command_with_redirection(env, cmd, args);
	}
	else
	{
		printf("Error: No valid command found to execute.\n");
	}
}

void	execute_redirection(t_env *env, char *command)
{
	char	*token;
	char	*cmd;
	char	*args;

	cmd = NULL;
	args = NULL;
	token = ft_strtok(command, " ");
	while (token != NULL)
	{
		handle_token(token, &cmd, &args);
		token = ft_strtok(NULL, " ");
	}
	execute_parsed_command(env, cmd, args);
}
