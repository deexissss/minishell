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

void ft_checker(char *command)
{
    if (memcmp(command, "cd", 2) == 0)
        execute_cd(command);
    else if (memcmp(command, "ls", 2) == 0)
        execute_ls();
    else if (memcmp(command, "rm", 2) == 0)
        execute_rm(command);
    else if (memcmp(command, "cat", 3) == 0)
        execute_cat(command);
    else if (memcmp(command, "env", 3) == 0)
        execute_env();
    else if (memcmp(command, "exit", 4) == 0)
        execute_exit(command);
    else if (memcmp(command, "echo", 4) == 0)
        execute_echo(command);
    else if (memcmp(command, "pwd", 4) == 0)
        execute_pwd();
    else if (memcmp(command, "sleep", 5) == 0)
        execute_sleep(command);
    else if (memcmp(command, "ls -l", 5) == 0)
        execute_lsl();
    else if (memcmp(command, "unset", 5) == 0)
        execute_unset(command);
    else if (memcmp(command, "touch", 5) == 0)
        execute_touch(command);
    else if (memcmp(command, "export", 6) == 0)
        execute_export(command);
}

void execute_commands(char *inpt)
{
    char *command;
    int i;
    int j;
    int len;
    int end;

    i = 0;
    j = 0;
    len = ft_strlen(inpt);
    while (i < len)
    {
        while (i < len && (inpt[i] == ' ' || inpt[i] == '\t'))
            i++;
        j = i;
        while (j < len && !(inpt[j] == '&' && inpt[j + 1] == '&'))
            j++;
        command = ft_strndup(inpt + i, j - i);
        end = ft_strlen(command) - 1;
        while (end >= 0 && (command[end] == ' ' || command[end] == '\t'))
            command[end--] = '\0';
        ft_checker(command);
        free(command);
        i = j + 2;
    }
}

char *execute_pwdmain()
{
    static char cwd[50000];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
        return cwd;
    else
    {
        perror("getcwd");
        return NULL;
    }
}

void handle_sigint(int sig)
{
    (void)sig;
    printf("\n");
    printf(BLUE "-> Minishell$ %s " RESET, execute_pwdmain());
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

int main()
{
    char *inpt;

    signal(SIGINT, handle_sigint);
    while (1)
    {
        printf(BLUE "-> Minishell$ %s " RESET, execute_pwdmain());
        inpt = readline("");
        if (!inpt)
            break;
        execute_commands(inpt);
        syntax_error(inpt);
        add_history(inpt);
        free(inpt);
    }
    return 0;
}

/*
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
}*/

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