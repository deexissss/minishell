/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 14:50:52 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/22 15:50:40 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_quoted(const char *str, int pos)
{
	bool	single_quote;
	bool	double_quote;
	int		i;

	single_quote = false;
	double_quote = false;
	i = 0;
	while (i < pos)
	{
		if (str[i] == '\'' && !double_quote)
			single_quote = !single_quote;
		else if (str[i] == '\"' && !single_quote)
			double_quote = !double_quote;
		i++;
	}
	return (single_quote || double_quote);
}

char	*handle_command(t_env *env, char *command)
{
	char	*expanded_command;
	int		end;

	expanded_command = handle_dollar(env, command);
	if (expanded_command == NULL)
	{
		free(command);
		return (NULL);
	}
	free(command);
	command = expanded_command;
	end = ft_strlen(command) - 1;
	while (end >= 0 && (command[end] == ' ' || command[end] == '\t')
		&& !is_quoted(command, end))
		command[end--] = '\0';
	return (command);
}

int	check_quote(t_env *env, char quote)
{
	if (quote != 0)
	{
		printf("error: unmatched quote\n");
		env->exit_status = 130;
		return (1);
	}
	return (0);
}

static int	check_command(t_env *env, char *command)
{
	int		k;
	char	quote;

	k = 0;
	quote = 0;
	while (command[k] != '\0')
	{
		if (command[k] == '"' || command[k] == '\'')
		{
			if (quote == 0)
				quote = command[k];
			else if (quote == command[k])
				quote = 0;
		}
		else if ((command[k] == ';' || command[k] == '\\') && quote == 0)
		{
			printf("error: syntax error\n");
			env->exit_status = 1;
			return (1);
		}
		k++;
	}
	if (check_quote(env, quote) == 1)
		return (1);
	return (0);
}

void	execute_commands(t_env *env, char *inpt)
{
	int		i;
	int		j;
	int		len;
	char	*command;

	i = 0;
	len = ft_strlen(inpt);
	while (i < len)
	{
		while (i < len && (inpt[i] == ' ' || inpt[i] == '\t'))
			i++;
		j = i;
		while (j < len && !(inpt[j] == '&' && inpt[j + 1] == '&'
				&& !is_quoted(inpt, j)))
			j++;
		command = ft_strndup(inpt + i, j - i);
		command = handle_command(env, command);
		if (check_command(env, command) == 0)
			process_command(env, command);
		free(command);
		i = j + 2;
	}
	free(inpt);
}
