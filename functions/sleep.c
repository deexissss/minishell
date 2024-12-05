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
		printf("error: memory allocation error\n");
		exit(1);
	}
	args[0] = "sleep";
	args[1] = time_str;
	args[2] = NULL;
	if (execvp(args[0], args) == -1)
	{
		printf("error: execvp failed\n");
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
		printf("error: sleep needs a time in seconds\n");
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		printf("error: fork failed\n");
		return ;
	}
	else if (pid == 0)
		handle_child(time);
	else
		waitpid(pid, NULL, 0);
}
