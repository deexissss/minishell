/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 14:47:52 by tjehaes           #+#    #+#             */
/*   Updated: 2025/02/06 09:53:52 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	correct_command(t_env *env, char **args)
{
	int	arg_count;
	int	i;

	i = 0;
	arg_count = 0;
	if (args)
	{
		while (args[arg_count])
			arg_count++;
		if (arg_count == 1)
			return (1);
		env->exit_status = 0;
		if (access(args[1], F_OK) != -1)
		{
			while (ft_strstr(args[i++], "grep") == NULL)
				return (1);
		}
		env->exit_status = 2;
	}
	return (1);
}

char	*command_path(t_env *env, char *command_name)
{
	char	*path;
	char	*env_path;

	if (ft_strncmp(command_name, "/bin/", 5) == 0)
		path = ft_strdup(command_name);
	else
	{
		env_path = get_env_value(env, "PATH");
		if (env_path && ft_strstr(env_path, "/bin"))
			path = ft_strjoin("/bin/", command_name);
		else
			path = NULL;
		free(env_path);
	}
	if (!path)
	{
		printf("error: command not found %s\n", command_name);
		env->exit_status = 127;
		free(path);
	}
	return (path);
}

bool	is_path_set(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->size)
	{
		if (ft_strncmp(env->variables[i], "PATH=", 5) == 0)
			return (true);
		i++;
	}
	return (false);
}

void	clear_terminal(char *command)
{
	int	i;

	i = 0;
	while (command[i] >= 'a' && command[i] <= 'z' && i < 5)
		i++;
	if (command[i] >= 'a' && command[i] <= 'z' && i == 5)
	{
		printf("error: clear miswrite\n");
		return ;
	}
	while (command[i] == ' ' || command[i] == '\t')
		i++;
	if (command[i] != '\0')
		printf("error: clear does not take argument\n");
	else if (isatty(STDOUT_FILENO))
		printf("\033[H\033[J");
	return ;
}

void	ft_checker(t_env *env, char *command)
{
	if (env->exit_status != 0)
		return ;
	if (ft_memcmp(command, "cd", 2) == 0)
		execute_cd(env, command);
	else if (ft_memcmp(command, "clear", 5) == 0)
		clear_terminal(command);
	else if (ft_memcmp(command, "env", 3) == 0)
		execute_env(env, command);
	else if (ft_memcmp(command, "exit", 4) == 0)
		execute_exit(env, command);
	else if (ft_memcmp(command, "echo", 4) == 0)
		cleanup_string(env, command);
	else if (ft_memcmp(command, "pwd", 3) == 0)
		execute_pwd(env, command);
	else if (ft_memcmp(command, "unset", 5) == 0)
		execute_unset(env, command);
	else if (ft_memcmp(command, "export", 6) == 0)
		execute_export(env, command);
	else
		handle_external_command(env, command);
}
