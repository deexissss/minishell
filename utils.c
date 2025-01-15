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
    int		i;
    char	*env_var;
    char	*value;

    i = 0;
    while (environ[i])
    {
        env_var = ft_strndup(environ[i], ft_strlen(var));
        if (ft_strncmp(env_var, var, ft_strlen(var)) == 0 && environ[i][ft_strlen(var)] == '=')
        {
            value = ft_strdup(&environ[i][ft_strlen(var) + 1]);
            free(env_var); // Free the temporary allocation
            return (value);
        }
        free(env_var); // Free the temporary allocation
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
	//rl_redisplay();
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
