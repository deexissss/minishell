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
/*
void ft_checker(char *command)
{
    if (memcmp(command, "cd", 2) == 0)
        execute_cd(command);
    else if (memcmp(command, "env", 3) == 0)
        execute_env();
    else if (memcmp(command, "exit", 4) == 0)
        execute_exit(command);
    else if (memcmp(command, "echo", 4) == 0)
        execute_echo(command);
    else if (memcmp(command, "pwd", 4) == 0)
        execute_pwd();
    else if (memcmp(command, "unset", 5) == 0)
        execute_unset(command);
    else if (memcmp(command, "export", 6) == 0)
        execute_export(command);
    else
    {

    }
}*/



/*
void execute_commands(char *inpt)
{
    char    *command;
    char    *clean_command;
    char    *expanded_command;
    int     i;
    int     j;
    int     k;
    int     len;
    int     end;
    int     flag;

    i = 0;
    j = 0;
    k = 0;
    len = ft_strlen(inpt);
    flag = 0;
    while (i < len)
    {
        while (i < len && (inpt[i] == ' ' || inpt[i] == '\t'))
            i++;
        j = i;
        while (j < len && !(inpt[j] == '&' && inpt[j + 1] == '&'))
            j++;
        command = ft_strndup(inpt + i, j - i);
        expanded_command = handle_dollar(command);
        free (command);
        end = ft_strlen(command) - 1;
        command = expanded_command;
        while (end >= 0 && (command[end] == ' ' || command[end] == '\t'))
            command[end--] = '\0';
        while (command[k] != '\0')
        {
            if (command[k] == ';' || command[k] == '\\')
            {
                ft_printf("minishell: syntax error near unexpected char\n");
                flag = 1;
                break;
            }
            k++;
        }
        if (flag == 0)
        {
            clean_command = cleanup_string(command);
            free(command);
            if (clean_command)
            {
                ft_checker(clean_command);
                free(clean_command);
            }
        }
        i = j + 2;
    }
}*/

bool is_quoted(const char *str, int pos)
{
    int i;
    bool single_quote = false;
    bool double_quote = false;

    i = 0;
    while(i < pos)
    {
        if (str[i] == '\'' && !double_quote)
        {
            single_quote = !single_quote;
        }
        else if (str[i] == '\"' && !single_quote)
        {
            double_quote = !double_quote;
        }
        i++;
    }
    return single_quote || double_quote;
}

char *handle_command(char *command)
{
    char *expanded_command;
    int end;

    expanded_command = handle_dollar(command);
    free(command);
    command = expanded_command;
    end = ft_strlen(command) - 1;
    while (end >= 0 && (command[end] == ' ' || command[end] == '\t') && !is_quoted(command, end))
        command[end--] = '\0';
    return command;
}

static int check_command(char *command)
{
    int k = 0;

    while (command[k] != '\0')
    {
        if (command[k] == ';' || command[k] == '\\')
        {
            printf("error: syntax error\n");
            return 1;
        }
        k++;
    }
    return 0;
}

static void process_command(char *command)
{
    char    *clean_command;
    char    **commands;
    int     num_commands;
    int     i;

    i = 0;
    clean_command = cleanup_string(command);
    free(command);
    if (clean_command)
    {
        //check for pipe
        if (ftstrchr(clean_command, '|' && !is_quoted(clean_command, '|')))
        {
            commands = pipe_tokenizer(clean_command, &num_commands);
            if (commands)
            {
                execute_pipeline(commands, num_commands);
                while (i < num_commands)
                {
                    free(commands[i]);
                    i++;
                }
                free(commands);
            }
        }
        else
        {
        ft_checker(clean_command);
        free(clean_command);
        }
    }
}

void execute_commands(char *inpt)
{
    int i;
    int j;
    int len;
    char *command;

    i = 0;
    len = ft_strlen(inpt);
    while (i < len)
    {
        while (i < len && (inpt[i] == ' ' || inpt[i] == '\t'))
            i++;
        j = i;
        while (j < len && !(inpt[j] == '&' && inpt[j + 1] == '&' && !is_quoted(inpt, j)))
            j++;
        command = ft_strndup(inpt + i, j - i);
        command = handle_command(command);
        if (check_command(command) == 0)
            process_command(command);
        else
            free(command);
        i = j + 2;
    }
}

int    handle_quote(char *inpt)
{
    int i;
    int countsimple;
    int countdouble;

    i = 0;
    countsimple = 0;
    countdouble = 0;
    while (inpt[i] != '\0')
    {
        if (inpt[i] == '\'')
            countsimple++;
        else if (inpt[i] == '\"')
            countdouble++;
        i++;
    }
    if (countsimple % 2 != 0 || countdouble % 2 != 0)
    {
        ft_printf("minishell: syntax error near unexpected char\n");
        return 1;
    }
    return 0;
}


int main()
{
    char *inpt;

    signal(SIGINT, handle_sigint);
    while (1)
    {
        printf(BLUE "-> Minishell %s " RESET, execute_pwdmain());
        inpt = readline(BLUE "$ " RESET);
        if (!inpt)
            break;
        if (ft_strlen(inpt) == 0)
            free(inpt);
        if (handle_quote(inpt) == 0)
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