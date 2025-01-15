/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdaman <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 11:01:52 by mdaman            #+#    #+#             */
/*   Updated: 2025/01/14 11:01:58 by mdaman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_perror(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void	pipe_check(int *pipefd)
{
	if (pipe(pipefd) == -1)
		exit_perror("pipe");
}

void	pid_check(pid_t pid)
{
	if (pid == -1)
		exit_perror("fork");
}

int		check_multiple_pipe(char *inpt)
{
	int		i;

	i = ft_strlen(inpt);
	if (inpt[i - 1] == '|')
	{
		g_exit_status = 1;
		printf("error: pipe alone\n");
		return (1);
	}
	i = 0;
	while (inpt[i])
	{
		if (inpt[i] == '|' && inpt[i+1] == '|'
			&& !is_pipe_inside_quotes(inpt))
		{
			g_exit_status = 1;
			printf("error: too many pipe\n");
			return (1);
		}
		else
			i++;
	}
	return (0);
}
