/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg.l    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 09:12:55 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/13 14:49:55 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*get_env_value(const char *var)
{
	int	i;
	int	var_len;

	i = 0;
	var_len = ft_strlen(var);
	while (g_env.variables[i] != NULL)
	{
		if (ft_strncmp(g_env.variables[i], var, var_len) == 0
			&& g_env.variables[i][var_len] == '=')
		{
			return (ft_strdup(g_env.variables[i] + var_len + 1));
		}
		i++;
	}
	return (NULL);
}

void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 2);
	rl_on_new_line();
	rl_replace_line("", 0);
	// rl_redisplay();
}

int	ft_strcmp(const char *str1, const char *str2)
{
	int	i;
	int	result;

	i = 0;
	result = 0;
	while (str1[i] != '\0' && str2[i] != '\0')
	{
		result = str1[i] - str2[i];
		if (result != 0)
			return (result);
		i++;
	}
	if (str1[i] == '\0' && str2[i] != '\0')
		return (0 - str2[i]);
	else if (str1[i] != '\0' && str2[i] == '\0')
		return (str1[i]);
	return (0);
}
