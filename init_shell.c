#include "minishell.h"

void	init_struct(void)
{
	int			i;
	extern char	**environ;

	i = 0;
	// Initialize g_env.variables from environ
	g_env.size = 0;
	g_env.exit_status = 0;
	while (environ[g_env.size] != NULL)
		g_env.size++;
	g_env.variables = malloc(sizeof(char *) * (g_env.size + 1));
	if (!g_env.variables)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	while (i < g_env.size)
	{
		g_env.variables[i] = ft_strdup(environ[i]);
		i++;
	}
	g_env.variables[i] = NULL;
}

void	free_struct(void)
{
	int	i;

	i = 0;
	while (g_env.variables[i] != NULL)
	{
		free(g_env.variables[i]);
		i++;
	}
	free(g_env.variables);
}
void	init_shell(int *saved_stdin, int *saved_stdout)
{
	init_struct();
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	rl_bind_key('\t', handle_tab);
	rl_bind_key(127, handle_backspace);
	using_history();
}