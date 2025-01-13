#include "minishell.h"

bool	is_redirection_inside_quotes(const char *str)
{
	bool	in_single_quote;
	bool	in_double_quote;

	in_single_quote = false;
	in_double_quote = false;
	while (*str)
	{
		if (*str == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*str == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (*str == '<' && !in_single_quote && !in_double_quote)
			return (false);
		else if (*str == '>' && !in_single_quote && !in_double_quote)
			return (false);
		str++;
	}
	return (true);
}

void	handle_token(char *token, char **cmd, char **args)
{
	bool	append;
	bool	is_heredoc;

    if (ft_strcmp(token, ">") == 0 || ft_strcmp(token, ">>") == 0)
    {
        append = ft_strcmp(token, ">>") == 0; 
        token = ft_strtok(NULL, " ");
        if (token != NULL)
            handle_output_redirection(token, append);
    }
	else if (ft_strcmp(token, "<") == 0 || ft_strcmp(token, "<<") == 0)
	{
		is_heredoc = ft_strcmp(token, "<<") == 0;
		token = ft_strtok(NULL, " ");
		if (token != NULL)
		{
			if (is_heredoc)
				handle_heredoc_redirection(token);
			else
				handle_input_redirection(token);
		}
	}
    else
        handle_command_or_args(token, cmd, args);
}

void	execute_command_with_redirection(char *cmd, char *args)
{
	char	*full_command;

	if (cmd != NULL)
	{
		size_t full_command_length = ft_strlen(cmd)
			+ (args ? ft_strlen(args) : 0) + 2;
			// checks if args is not null(if so return 0),then adds the length of args to the length of cmd
		full_command = malloc(full_command_length);
		ft_strcpy(full_command, cmd);
		if (args)
		{
			ft_strlcat(full_command, " ", full_command_length);
			ft_strlcat(full_command, args, full_command_length);
		}
		ft_checker(full_command);
		free(full_command);
		free(cmd);
		if (args)
			free(args);
	}
}

void	execute_parsed_command(char *cmd, char *args)
{
    if (cmd != NULL)
    {
        if (ftstrchr(cmd, '|') && !is_pipe_inside_quotes(cmd))
        {
            int num_commands;
            char **commands = pipe_tokenizer(cmd, &num_commands);
            if (commands)
            {
                execute_pipeline(commands, num_commands);
                for (int i = 0; i < num_commands; i++)
                    free(commands[i]);
                free(commands);
            }
        }
        else
            execute_command_with_redirection(cmd, args);
    }
    else
    {
        printf("Error: No valid command found to execute.\n");
    }
}
void	execute_redirection(char *command)
{
	char *token;
	char *cmd = NULL;
	char *args = NULL;

	token = ft_strtok(command, " ");
	while (token != NULL)
	{
		handle_token(token, &cmd, &args);
		token = ft_strtok(NULL, " ");
	}
	execute_parsed_command(cmd, args);
}
