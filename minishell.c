/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 12:53:38 by tjehaes           #+#    #+#             */
/*   Updated: 2024/11/12 15:10:51 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"

/*void execute_lsl()
{
    char *args[] = {"/bin/ls", "-l", NULL};
    int val = execve(args[0], args, NULL);

    if (val == -1) {
        perror("execve");
    }
}

void    ft_checker(char *inpt)
{
        if (ft_memcmp(inpt, "cd", 2) == 0)
                printf("cd");
        if (ft_memcmp(inpt, "ls -l", 5) == 0)
                execute_lsl();
}
int main()
{
        char *inpt;

        while (1)
        {
                inpt = readline(BLUE"-> Minishell$ "RESET);
                ft_checker(inpt);
                add_history(inpt);
                printf("\n");
        }
        return 0;
}*/

void    syntax_error(char *inpt)
{
    if (memcmp(inpt, ">", 2) == 0)
        ft_printf("syntax error");
    if (memcmp(inpt, "<", 2) == 0)
        ft_printf("syntax error");
    if (memcmp(inpt, ">>", 2) == 0)
        ft_printf("syntax error");
    if (memcmp(inpt, "<<", 2) == 0)
        ft_printf("syntax error");
    if (memcmp(inpt, "<>", 2) == 0)
        ft_printf("syntax error");
}
  
void    execute_lsl()
{
    char *args[] = {"/bin/ls", "-l", NULL};
    pid_t pid = fork();

    if (pid == -1)
        perror("fork");
    else if (pid == 0)
    {
        if (execve(args[0], args, NULL) == -1)
        {
            perror("execve");
            _exit(1);
        }
    }
    else
        waitpid(pid, NULL, 0);
}

void    execute_ls()
{
    char *args[] = {"/bin/ls", NULL};
    pid_t pid = fork();

    if (pid == -1)
        perror("fork");
    else if (pid == 0)
    {
        if (execve(args[0], args, NULL) == -1)
        {
            perror("execve");
            _exit(1);
        }
    }
    else
        waitpid(pid, NULL, 0);
}

void    execute_pwd()
{
    char *args[] = {"/bin/pwd", NULL};
    pid_t pid = fork();

    if (pid == -1)
        perror("fork");
    else if (pid == 0)
    {
        if (execve(args[0], args, NULL) == -1)
        {
            perror("execve");
            _exit(1);
        }
    }
    else 
        waitpid(pid, NULL, 0);
}

void    execute_echo(char *inpt)
{
    int     i;

    i = 4;
    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    while (inpt[i])
    {
        write(1, &inpt[i], 1);
        i++;
    }
}

void    execute_sleep(char *inpt)
{
    int     i;
    int     time;
    pid_t pid = fork();

    i = 5;
    time = 0;
    while (inpt[i] == ' ' || inpt[i] == '\t')
        i++;
    if (inpt[i] == '\0')
        ft_printf("error : sleep need a time");
    while (inpt[i] >= '0' && inpt[i] <= '9')
    {
        time = time * 10 + (inpt[i] - '0');
        i++;
    }
    if (pid == -1)
        perror("fork");
    else if (pid == 0)
    {
        char time_str[20];
        snprintf(time_str, sizeof(time_str), "%d", time);
        char *args[] = {"sleep", time_str, NULL};
        if (execvp(args[0], args) == -1)
        {
            perror("execvp");
            _exit(1);
        }
    }
    else
        waitpid(pid, NULL, 0);
}


void ft_checker(char *inpt)
{
    if (memcmp(inpt, "cd", 2) == 0)
        printf("cd");
    if (memcmp(inpt, "ls -l", 5) == 0)
        execute_lsl();
    if (memcmp(inpt, "ls", 2) == 0)
        execute_ls();
    if (memcmp(inpt, "echo", 4) == 0)
        execute_echo(inpt);
    if (memcmp(inpt, "pwd", 4) == 0)
        execute_pwd();
    if (memcmp(inpt, "sleep", 5) == 0)
        execute_sleep(inpt);
}

int main()
{
    char *inpt;

    while (1)
    {
        inpt = readline(BLUE "-> Minishell$ " RESET);
        if (!inpt)
            break;
        syntax_error(inpt);
        ft_checker(inpt);
        add_history(inpt);
        printf("\n");
        free(inpt);
    }
    return 0;
}
/*
void	ft_initialize(t_data *data)
{
	data->is_pipe = 0;
	data->pipe_locale = NULL;
	data->command_num = 0;
	data->command = NULL;
	data->in_out_fd[0] = -1;
	data->in_out_fd[1] = -1;
	data->in_out_fd[2] = -1;
	data->in_out_fd[3] = -1;
	data->in_out_fd[4] = -1;
	data->in_out_fd[5] = -1;
	data->table = malloc(sizeof(t_list *) * (128));//A hash table with 128 buckets can handle a moderate number of entries efficiently
	//still needs the memory allocation fail check including freeing all the data
	for (int i = 0; i < 128; i++)
		data->table[i] = NULL;
}

int	main(int result, char **av, char **env)
{
	t_data	data;

	av[0] = NULL;
	while (1)
	{
		ft_init(&data); //this could also be handled with libft (bzero? not sure which function)
		//parsing/reading (which at the same time handles the history?)
		//free/exit
		
	}
	//free/exit everything
	return (0);
}*/