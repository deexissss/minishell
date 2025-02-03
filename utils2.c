/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 14:50:39 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/22 15:50:46 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*clean_string(char *str, int i, int j, char *clean_str)
{
	char	quote;
	int		find_quote;

	find_quote = 0;
	while (str[i])
	{
		if (find_quote == 0 && (str[i] == '\'' || str[i] == '"'))
		{
			find_quote = 1;
			quote = str[i];
		}
		else if (find_quote == 1 && str[i] == quote)
		{
			find_quote = 0;
			quote = '\0';
		}
		else
		{
			clean_str[j] = str[i];
			j++;
		}
		i++;
	}
	clean_str[j] = '\0';
	return (clean_str);
}

char	*cleanup_string(t_env *env, char *str)
{
	char	*clean_str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	clean_str = malloc(ft_strlen(str) + 1);
	if (!clean_str)
		return (NULL);
	if (ft_memcmp(str, "echo", 4) == 0)
	{
		execute_echo(env, str);
		free(clean_str);
		return (NULL);
	}
	clean_str = clean_string(str, i, j, clean_str);
	return (clean_str);
}

char	*int_to_str(int num, char *str)
{
	int	len;
	int	temp;

	len = 1;
	temp = num;
	while (temp / 10 != 0)
	{
		len++;
		temp /= 10;
	}
	str[len] = '\0';
	while (len > 0)
	{
		len--;
		str[len] = (num % 10) + '0';
		num /= 10;
	}
	return (str);
}

void	exec_func(t_env *env, char *path, char **args)
{
	signal(SIGQUIT, SIG_DFL);
	if (execve(path, args, env->variables) == -1)
	{
		if (env->exit_status == 0)
			env->exit_status = 1;
		exit(EXIT_FAILURE);
	}
}

int	is_line_empty(const char *line)
{
	while (*line)
	{
		if (!isspace((unsigned char)*line))
			return (0);
		line++;
	}
	return (1);
}
