/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 14:47:52 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/13 14:48:59 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	correct_command(char **args)
{
	if (!args || !args[0])
	{
		printf("error: command not found\n");
		g_exit_status = 127;
		if (args)
		{
			g_exit_status = 1;
			free(args);
		}
		return (0);
	}
	return (1);
}

char	*command_path(char *command_name)
{
	char	*path;

	if (ft_strncmp(command_name, "/bin/", 5) == 0)
		path = ft_strdup(command_name);
	else
		path = ft_strjoin("/bin/", command_name);
	if (!path)
		perror("malloc");
	return (path);
}

void	execute_command(char *path, char **args)
{
	if (execve(path, args, NULL) == -1)
	{
		g_exit_status = 1;
		perror("minishell");
		exit(EXIT_FAILURE);
	}
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
