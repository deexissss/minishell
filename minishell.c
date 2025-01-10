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
    //printf("expanded_command: %s\n", expanded_command);
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
    char quote = 0;

    while (command[k] != '\0')
    {
        if (command[k] == '"' || command[k] == '\'')
        {
            if (quote == 0)
                quote = command[k];
            else if (quote == command[k])
                quote = 0;
        }
        else if ((command[k] == ';' || command[k] == '\\') && quote == 0)
        {
            printf("error: syntax error\n");
            exit_status = 1;
            return 1;
        }
        k++;
    }

    if (quote != 0)
    {
        printf("error: unmatched quotes\n");
        exit_status = 130;
        return 1;
    }

    return 0;
}
/*static int check_command(char *command)
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
}*/

static void process_command(char *command)
{
    char    *clean_command;
    char    **commands;
    int     num_commands;
    int     i;

    i = 0;
    if (command)
    {
        if (!is_pipe_inside_quotes(command))
        {
            // Tokenize the command by pipes first
            commands = pipe_tokenizer(command, &num_commands);
            if (commands)
            {
                if (num_commands > 1 && !is_pipe_inside_quotes(command))
                {
                    // If there are multiple commands, execute them in a pipeline
                    execute_pipeline(commands, num_commands);
                }
                else
                {
                    // Check for redirections in the single command segment
                    if ((ftstrchr(commands[i], '>') || ftstrchr(commands[i], '<')) && !is_redirection_inside_quotes(commands[i]))
                        execute_redirection(commands[i]);
                    else
                    {
                        clean_command = cleanup_string(commands[i]);
                        if (clean_command)
                        {
                            ft_checker(clean_command);
                            free(clean_command);
                        }
                    }
                }
                i = 0;
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
            // If there is a pipe inside quotes, execute the command as is
            clean_command = cleanup_string(command);
            if (clean_command)
            {
                ft_checker(clean_command);
                free(clean_command);
            }
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
        ft_printf("minishell: syntax error quotes not closed\n");
        exit_status = 130;
        return 1;
    }
    return 0;
}


int check_empty_functions(char *inpt)
{
    int i;

    i = 0;
    while (inpt[i] != '\0')
    {
        if (inpt[i] == ' ' || inpt[i] == '\t' || inpt[i] == '\0')
            i++;
        else
            return 1;
    }
    return 0;
}
int handle_tab(int count, int key)
{
    (void)count;
    (void)key;
    rl_insert_text("\t");
    rl_redisplay();
    return 0;
}

int handle_backspace(int count, int key)
{
    (void)count;
    (void)key;
    if(rl_point > 0)
    {
        rl_delete_text(rl_point - 1, rl_point);
        rl_redisplay();
    }
    return 0;
}

int main()
{
    char *inpt;
    int saved_stdin = dup(STDIN_FILENO);
    int saved_stdout = dup(STDOUT_FILENO);

    signal(SIGINT, handle_sigint);
    rl_bind_key('\t', handle_tab);
    rl_bind_key(127, handle_backspace);
    while (1)
    {
        inpt = readline(BLUE "Minishell$ " RESET);
        if (!inpt)
            break;
        if (check_empty_functions(inpt) == 0)
            free(inpt);
        else if (handle_quote(inpt) == 0)
            execute_commands(inpt);
        syntax_error(inpt);
        add_history(inpt);
        //free(inpt);
        // Restore original file descriptors
        dup2(saved_stdin, STDIN_FILENO);
        dup2(saved_stdout, STDOUT_FILENO);
    }
    close(saved_stdin);
    close(saved_stdout);
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


/*void	ft_initialize(t_data *data)
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