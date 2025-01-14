#include "minishell.h"

void	exit_perror(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void	pipe_check(int *pipefd)
{
	if (pipe(pipefd) == -1)
		exit_perror("pipe");
}

void	pid_check(pid_t pid)
{
	if (pid == -1)
		exit_perror("fork");
}