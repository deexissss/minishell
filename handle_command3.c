#include "minishell.h"

void	exec_perror(char *str)
{
	perror(str);
	g_env.exit_status = 1;
}

void	update_exit_status(int status)
{
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		g_env.exit_status = (WEXITSTATUS(status));
}

void	handle_ls_command(char **args)
{
	struct stat	buffer;
	int			exists;
	int			i;

	i = 1;
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

void	execute_command(char *path, char **args)
{
	if (!is_path_set())
	{
		g_env.exit_status = 127;
		write(STDERR_FILENO, "minishell: command not found\n", 29);
		return ;
	}
	if (ft_strncmp(path, "ls", 2) == 0 || ft_strncmp(path, "/bin/ls", 7) == 0)
	{
		handle_ls_command(args);
	}
	g_env.exit_status = 0;
	exec_func(path, args);
}