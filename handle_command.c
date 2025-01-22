/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 14:47:52 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/15 09:29:01 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	correct_command(char **args)
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
		g_env.exit_status = 0;
		if (access(args[1], F_OK) != -1)
		{
			while (ft_strstr(args[i++], "grep") == NULL)
				return (1);
		}
		g_env.exit_status = 2;
	}
	return (1);
}

char	*command_path(char *command_name)
{
	char	*path;
	char	*env_path;

	if (ft_strncmp(command_name, "/bin/", 5) == 0)
		path = ft_strdup(command_name);
	else
	{
		env_path = get_env_value("PATH");
		if (env_path && ft_strstr(env_path, "/bin"))
			path = ft_strjoin("/bin/", command_name);
		else
			path = NULL;
		free(env_path);
	}
	if (!path)
	{
		printf("error: command not found %s\n", command_name);
		g_env.exit_status = 127;
		free(path);
	}
	return (path);
}

bool	is_path_set(void)
{
	int	i;

	i = 0;
	while (i < g_env.size)
	{
		if (ft_strncmp(g_env.variables[i], "PATH=", 5) == 0)
			return (true);
		i++;
	}
	return (false);
}

void	execute_command(char *path, char **args)
{
	struct stat	buffer;
	int			exists;
	int			i;

	if (!is_path_set())
	{
		g_env.exit_status = 127;
		write(STDERR_FILENO, "minishell: command not found\n", 29);
		return ;
	}
	i = 1;
	if (ft_strncmp(path, "ls", 2) == 0 || ft_strncmp(path, "/bin/ls", 7) == 0)
	{
		while (args[i] != NULL)
		{
			exists = stat(args[i], &buffer);
			if (exists != 0)
			{
				g_env.exit_status = 2;
				write(STDERR_FILENO, "minishell: ", 11);
				write(STDERR_FILENO, args[i], ft_strlen(args[i]));
				write(STDERR_FILENO, ": No such file or directory\n", 28);
				return ;
			}
			i++;
		}
	}
	g_env.exit_status = 0;
	exec_func(path, args);
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

void	ft_checker(char *command)
{
	if (ft_memcmp(command, "cd", 2) == 0)
		execute_cd(command);
	else if (ft_memcmp(command, "clear", 5) == 0)
		clear_terminal(command);
	else if (ft_memcmp(command, "env", 3) == 0)
		execute_env(command);
	else if (ft_memcmp(command, "exit", 4) == 0)
		execute_exit(command);
	else if (ft_memcmp(command, "echo", 4) == 0)
		cleanup_string(command);
	else if (ft_memcmp(command, "pwd", 3) == 0)
		execute_pwd(command);
	else if (ft_memcmp(command, "unset", 5) == 0)
		execute_unset(command);
	else if (ft_memcmp(command, "export", 6) == 0)
		execute_export(command);
	else
		handle_external_command(command);
}
