/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdaman <mdaman@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:21:08 by mdaman            #+#    #+#             */
/*   Updated: 2025/01/22 13:21:35 by mdaman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigquit(int sig)
{
	struct termios	term;

	(void)sig;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	if (rl_line_buffer && !is_line_empty(rl_line_buffer))
	{
		write(STDOUT_FILENO, "\nQuit (core dumped)\n", 20);
		rl_replace_line("", 0);
		rl_redisplay();
		exit(131);
	}
}

int	check_quote_empty(char *line)
{
	if (ft_strlen(line) == 2 && line[0] == '"' && line[1] == '"')
	{
		printf("bash: : command not found\n");
		return (1);
	}
	else if (ft_strlen(line) == 2 && line[0] == '\'' && line[1] == '\'')
	{
		printf("bash: : command not found\n");
		return (1);
	}
	return (0);
}

int	print_error(char *line)
{
	printf("bash: %s: command not found\n", line);
	return (1);
}

int	check_check(char *line)
{
	int	indexes[2];
	int	i;

	indexes[0] = 0;
	indexes[1] = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == ' ' || line[i] == '\t')
			indexes[0]++;
		if (line[i] == '"' || line[i] == '\'')
			indexes[1]++;
		i++;
	}
	if (indexes[0] >= 1 && indexes[1] == 2 && line[0] == '"'
		&& line[ft_strlen(line) - 1] == '"')
		return (print_error(line));
	else if (indexes[0] >= 1 && indexes[1] == 2 && line[0] == '\''
		&& line[ft_strlen(line) - 1] == '\'')
		return (print_error(line));
	return (0);
}
