#include "../minishell.h"

/*void    execute_sleep(char *inpt)
{
    int     i;
    int     time;
    pid_t pid;
    char time_str[20];
    char *args[] = {"sleep", time_str, NULL};

    i = 5;
    time = 0;
    pid = fork();
    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    if (inpt[i] == '\0')
        perror("sleep");
    while (inpt[i] >= '0' && inpt[i] <= '9')
    {
        time = time * 10 + (inpt[i] - '0');
        i++;
    }
    if (pid == -1)
        perror("fork");
    else if (pid == 0)
    {
        snprintf(time_str, sizeof(time_str), "%d", time);
        if (execvp(args[0], args) == -1)
        {
            perror("execvp");
            _exit(1);
        }
    }
    else
        waitpid(pid, NULL, 0);
}*/

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
