/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:09:16 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/13 15:09:37 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strtok(char *str, const char *delimiters)
{
	static char	*stock = NULL;
	char		*ptr;

	if (str != NULL)
		stock = str;
	if (stock == NULL)
		return (NULL);
	while (*stock != '\0' && ftstrchr(delimiters, *stock) != NULL)
		stock++;
	if (*stock == '\0')
		return (NULL);
	ptr = stock;
	while (*stock != '\0' && ftstrchr(delimiters, *stock) == NULL)
		stock++;
	if (*stock != '\0')
	{
		*stock = '\0';
		stock++;
	}
	return (ptr);
}

char	*duplicate_command(char *command)
{
	char	*command_copy;

	command_copy = ft_strdup(command);
	if (!command_copy)
		perror("strdup");
	return (command_copy);
}

char	**allocate_commands_array(char *command)
{
	char	**commands;

	commands = malloc((ft_strlen(command) / 2 + 1) * sizeof(char *));
	if (!commands)
		perror("malloc");
	return (commands);
}

int	tokenize_command(char *command_copy, char **commands)
{
	int		i;
	char	*token;

	i = 0;
	token = ft_strtok(command_copy, "|");
	while (token != NULL)
	{
		commands[i] = ft_strdup(token);
		if (!commands[i])
		{
			perror("strdup");
			while (i > 0)
				free(commands[--i]);
			free(commands);
			return (-1);
		}
		i++;
		token = ft_strtok(NULL, "|");
	}
	commands[i] = NULL;
	return (i);
}

char	**pipe_tokenizer(char *command, int *num_commands)
{
	char	*command_copy;
	char	**commands;
	int		num_tokens;

	command_copy = duplicate_command(command);
	if (!command_copy)
		return (NULL);
	commands = allocate_commands_array(command);
	if (!commands)
	{
		free(command_copy);
		return (NULL);
	}
	num_tokens = tokenize_command(command_copy, commands);
	if (num_tokens == -1)
	{
		free(command_copy);
		return (NULL);
	}
	*num_commands = num_tokens;
	free(command_copy);
	return (commands);
}
