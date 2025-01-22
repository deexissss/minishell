/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 14:50:52 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/22 09:49:41 by codespace        ###   ########.fr       */
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

char	*handle_command(char *command)
{
	char	*expanded_command;
	int		end;

	expanded_command = handle_dollar(command);
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

int	check_quote(char quote)
{
	if (quote != 0)
	{
		printf("error: unmatched quote\n");
		g_env.exit_status = 130;
		return (1);
	}
	return (0);
}

static int	check_command(char *command)
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
			g_env.exit_status = 1;
			return (1);
		}
		k++;
	}
	if (check_quote(quote) == 1)
		return (1);
	return (0);
}


void	execute_commands(char *inpt)
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
		command = handle_command(command);
		if (check_command(command) == 0)
			process_command(command);
		free(command);
		i = j + 2;
	}
	free(inpt);
}