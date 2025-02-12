/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 14:22:38 by tjehaes           #+#    #+#             */
/*   Updated: 2025/02/07 14:23:14 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	g_sigint_received = 1;
	write(STDOUT_FILENO, "\n", 1);
	if (rl_line_buffer && *rl_line_buffer)
	{
		rl_replace_line("", 0);
		rl_redisplay();
	}
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
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

char	*get_env_value(t_env *env, const char *var)
{
	int	i;
	int	var_len;

	i = 0;
	var_len = ft_strlen(var);
	while (env->variables[i] != NULL)
	{
		if (ft_strncmp(env->variables[i], var, var_len) == 0
			&& env->variables[i][var_len] == '=')
		{
			return (ft_strdup(env->variables[i] + var_len + 1));
		}
		i++;
	}
	return (NULL);
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
