#include "../minishell.h"

static int	check_time(const char *input)
{
	int	i;
	int	time;

	i = 5;
	time = 0;
	while (input[i] == ' ' || input[i] == '\t')
		i++;
	if (input[i] == '\0')
		return (-1);
	while (input[i] >= '0' && input[i] <= '9')
	{
		time = time * 10 + (input[i] - '0');
		i++;
	}
	return (time);
}

static void	handle_child(int time)
{
	char	*time_str;
	char	*args[3];

	time_str = ft_itoa(time);
	if (!time_str)
	{
		write(2, "memory allocation error\n", 24);
		exit(1);
	}
	args[0] = "sleep";
	args[1] = time_str;
	args[2] = NULL;
	if (execvp(args[0], args) == -1)
	{
		write(2, "execvp: error\n", 14);
		free(time_str);
		exit(1);
	}
	free(time_str);
}

void	execute_sleep(char *input)
{
	int		time;
	pid_t	pid;

	time = check_time(input);
	if (time == -1)
	{
		write(2, "sleep: missing or invalid argument\n", 35);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		write(2, "fork: error\n", 12);
		return ;
	}
	else if (pid == 0)
		handle_child(time);
	else
		waitpid(pid, NULL, 0);
}
