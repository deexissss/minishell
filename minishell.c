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
		g_env.exit_status = 127;
		return (1);
	}
	return (0);
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
		g_env.exit_status = 130;
		return (1);
	}
	return (0);
}

void	process_input(char *input)
{
	// add_history(input);
	if (check_empty_functions(input) == 0 || simple_dollar(input) == 1
		|| check_multiple_pipe(input) == 1)
		free(input);
	else if (handle_quote(input) == 0)
		execute_commands(input);
	else
		free(input);
}
// int		g_env.exit_status = 0;
t_env	g_env;

int	main(void)
{
	char	*inpt;
	int		saved_stdin;
	int		saved_stdout;

	init_shell(&saved_stdin, &saved_stdout);
	while (1)
	{
		inpt = readline(BLUE "Minishell$ " RESET);
		if (!inpt)
			break ;
		if (*inpt)
			add_history(inpt);
		process_input(inpt);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		rl_replace_line("", 0);
		rl_redisplay();
	}
	close(saved_stdin);
	close(saved_stdout);
	free_struct();
	return (0);
}
