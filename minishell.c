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

char *get_env_value(const char *var)
{
    char *value = getenv(var);
    if (value)
        return strdup(value);
    else
        return NULL;
}

char *handle_dollar(char *command)
{
    char    *result;
    char    *var_start;
    char    *var_end;
    char    *var_value;
    int     i;
    int     j;

    result = malloc(ft_strlen(command) + 1);
    i = 0;
    j = 0;

    while(command[i] != '\0')
    {
        //check for $ and if it is not escaped
        if (command[i] == '$' && (i == 0 || command[i - 1] != '\\'))
        {
            var_start = &command[i +1];//points to the character after the $
            var_end = var_start;//takes the same position as var_start
            //traverses the variable name using the position of var_end
            while (*var_end && (ft_isalnum((int)*var_end) || *var_end == '_'))
                var_end++;
            char var_name[var_end - var_start + 1];
            ft_strlcpy(var_name, var_start, var_end - var_start); //copy temporarily the variable name
            var_name[var_end - var_start] = '\0';
            var_value = get_env_value(var_name);
            if (var_value)
            {
                ft_strcpy(&result[j], var_value);
                j += ft_strlen(var_value);
                free(var_value);
            }
            i = var_end - command;
        }
        // if the character is not a $ or the $ is escaped simply copy the character
        else
        result[j++] = command[i++];
    }
    result[j] = '\0';
    return (result);
}

char *cleanup_string(char *str)
{
    int i;
    int j;
    char *clean_str;

    i = 0;
    j = 0;
    clean_str = malloc(strlen(str) + 1);
    if (!clean_str)
        return NULL;
    while (str[i] != '\0')
    {
        if (str[i] != '\'' && str[i] != '\"')
        {
            clean_str[j] = str[i];
            j++;
        }
        i++;
    }
    clean_str[j] = '\0';
    return clean_str;
}

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
        //while ((j < len && !(inpt[j] == '&' && inpt[j + 1] == '&')) || (j < len && !(inpt[j] == ';')))
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

void handle_sigint(int sig)
{
    (void)sig;
    printf("\n");
    printf(BLUE "-> Minishell %s " RESET, execute_pwdmain());
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