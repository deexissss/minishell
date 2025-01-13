/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 12:53:38 by tjehaes           #+#    #+#             */
/*   Updated: 2024/11/12 15:10:51 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_quoted(const char *str, int pos)
{
	bool	single_quote = false;
	bool	double_quote = false;
	int		i = 0;
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
	free(command);
	command = expanded_command;
	end = ft_strlen(command) - 1;
	while (end >= 0 && (command[end] == ' ' || command[end] == '\t')
		&& !is_quoted(command, end))
		command[end--] = '\0';
	return (command);
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
			exit_status = 1;
			return (1);
		}
		k++;
	}
	if (quote != 0)
	{
		printf("error: unmatched quotes\n");
		exit_status = 130;
		return (1);
	}
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
		else
			free(command);
		i = j + 2;
	}
}

int	handle_quote(char *inpt)
{
	int	i;
	int	countsimple;
	int	countdouble;

	i = 0;
	countsimple = 0;
	countdouble = 0;
	while (inpt[i] != '\0')
	{
		if (inpt[i] == '\'')
			countsimple++;
		else if (inpt[i] == '\"')
			countdouble++;
		i++;
	}
	if (countsimple % 2 != 0 || countdouble % 2 != 0)
	{
		ft_printf("minishell: syntax error quotes not closed\n");
		exit_status = 130;
		return (1);
	}
	return (0);
}

int	check_empty_functions(char *inpt)
{
	int	i;

	i = 0;
	while (inpt[i] != '\0')
	{
		if (inpt[i] == ' ' || inpt[i] == '\t' || inpt[i] == '\0')
			i++;
		else
			return (1);
	}
	return (0);
}
int	handle_tab(int count, int key)
{
	(void)count;
	(void)key;
	rl_insert_text("\t");
	rl_redisplay();
	return (0);
}

int	handle_backspace(int count, int key)
{
	(void)count;
	(void)key;
	if (rl_point > 0)
	{
		rl_delete_text(rl_point - 1, rl_point);
		rl_redisplay();
	}
	return (0);
}

int	simple_dollar(char *inpt)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = ft_strlen(inpt);
	while (*inpt == '$')
	{
		count++;
		inpt++;
	}
	if (count == i)
	{
		ft_printf("error: command not found\n");
		exit_status = 127;
		return (1);
	}
	return (0);
}

int			exit_status = 0;

int	main(void)
{
	char	*inpt;
	int		saved_stdin;
	int		saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	signal(SIGINT, handle_sigint);
	rl_bind_key('\t', handle_tab);
	rl_bind_key(127, handle_backspace);
	while (1)
	{
		inpt = readline(BLUE "Minishell$ " RESET);
		if (!inpt)
			break ;
		if (check_empty_functions(inpt) == 0)
			free(inpt);
		else if (simple_dollar(inpt) == 1)
			free(inpt);
		else if (handle_quote(inpt) == 0)
			execute_commands(inpt);
		syntax_error(inpt);
		add_history(inpt);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
	}
	close(saved_stdin);
	close(saved_stdout);
	return (0);
}
