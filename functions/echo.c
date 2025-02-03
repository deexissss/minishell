/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:38:09 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/22 15:50:56 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static bool	check_n_option(char *input, int *i)
{
	bool	n_flag;
	int		temp;

	n_flag = false;
	while (input[*i] == '-')
	{
		temp = *i + 1;
		while (input[temp] == 'n')
			temp++;
		if (input[temp] == ' ' || input[temp] == '\t' || input[temp] == '\0')
		{
			n_flag = true;
			*i = skip_spaces(input, temp);
		}
		else
			break ;
	}
	return (n_flag);
}

static void	print_quote_word(char *input, int *i, char quote)
{
	(*i)++;
	while (input[*i] && input[*i] != quote)
		ft_printchar(input[(*i)++]);
	if (input[*i] == quote)
		(*i)++;
}

static void	print_word(char *input, int *i, bool *first_word)
{
	if (!(*first_word))
		write(1, " ", 1);
	*first_word = false;
	while (input[*i] && input[*i] != ' ' && input[*i] != '\t'
		&& input[*i] != '"' && input[*i] != '\'')
		ft_printchar(input[(*i)++]);
}

static void	print_words(char *input, int i)
{
	bool	first_word;
	char	quote;

	first_word = true;
	while (input[i])
	{
		if (input[i] == '"' || input[i] == '\'')
		{
			quote = input[i];
			print_quote_word(input, &i, quote);
		}
		else if (input[i] != ' ' && input[i] != '\t')
			print_word(input, &i, &first_word);
		else
			i = skip_spaces(input, i);
	}
}

void	execute_echo(t_env *env, char *input)
{
	int		i;
	bool	n_option;

	i = 4;
	env->exit_status = 0;
	if (ft_strncmp(input, "echo", 4) != 0)
	{
		printf("error: command need to start with 'echo'\n");
		env->exit_status = 127;
		return ;
	}
	if (input[i] != ' ' && input[i] != '\t' && input[i] != '\0')
	{
		printf("error: need a space between echo and the arguments\n");
		env->exit_status = 127;
		return ;
	}
	i = skip_spaces(input, i);
	n_option = check_n_option(input, &i);
	print_words(input, i);
	if (!n_option)
		printf("\n");
}
