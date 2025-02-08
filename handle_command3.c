/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:25:20 by mdaman            #+#    #+#             */
/*   Updated: 2025/02/06 09:55:27 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_perror(t_env *env, char *str)
{
	perror(str);
	env->exit_status = 1;
}

void	update_exit_status(t_env *env, int status)
{
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		env->exit_status = (WEXITSTATUS(status));
}

int	is_path_set_check(t_env *env)
{
	if (!is_path_set(env))
	{
		env->exit_status = 127;
		write(STDERR_FILENO, "minishell: command not found\n", 29);
		return (1);
	}
	return (0);
}

void	execute_command(t_env *env, char *path, char **args)
{
	struct stat	buffer;
	int			exists;
	int			i;

	i = 1;
	if (is_path_set_check(env) == 1)
		return ;
	if (ft_strncmp(path, "ls", 2) == 0 || ft_strncmp(path, "/bin/ls", 7) == 0)
	{
		while (args[i] != NULL)
		{
			exists = stat(args[i], &buffer);
			if (exists != 0)
			{
				env->exit_status = 2;
				write(STDERR_FILENO, "minishell: ", 11);
				write(STDERR_FILENO, args[i], ft_strlen(args[i]));
				write(STDERR_FILENO, ": No such file or directory\n", 28);
				return ;
			}
			i++;
		}
	}
	env->exit_status = 0;
	exec_func(env, path, args);
}
