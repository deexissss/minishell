/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:48:43 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/22 15:11:58 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	skip_space_and_sign(char *input, int *i, int *sign)
{
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
	if (input[*i] == '-' && input[*i + 1] != '-')
	{
		*sign = -1;
		(*i)++;
	}
}

int	give_status(char *input, int *i)
{
	int	status;

	status = 0;
	while (input[*i])
	{
		if (input[*i] >= '0' && input[*i] <= '9')
			status = status * 10 + (input[(*i)++] - '0');
		else if ((input[*i] >= 'a' && input[*i] <= 'z') || (input[*i] >= 'A'
				&& input[*i] <= 'Z'))
		{
			printf("error: numeric argumenbt required\n");
			return (-1);
		}
	}
	return (status);
}

int	count_arg(char *inpt)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (inpt[i])
	{
		if (inpt[i] != ' ' && inpt[i] != '\t')
		{
			count++;
			while (inpt[i] && inpt[i] != ' ' && inpt[i] != '\t')
				i++;
		}
		else
			i++;
	}
	return (count);
}

void	execute_exit(t_env *env, char *input)
{
	int	i;
	int	sign;
	int	status;
	int	count;

	i = 4;
	sign = 1;
	count = count_arg(input);
	skip_space_and_sign(input, &i, &sign);
	if (count > 2)
	{
		printf("error: too many arguments\n");
		env->exit_status = 1;
		return ;
	}
	status = give_status(input, &i);
	if (status >= 0)
	{
		printf("exit\n");
		exit(status * sign);
	}
	env->exit_status = 1;
}
