/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 14:51:27 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/15 09:31:33 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*allocate_result_buffer(char *command)
{
	char	*result;
	size_t	buffer_size;

	buffer_size = (ft_strlen(command) + 1) * 256;
	result = malloc(buffer_size);
	if (!result)
	{
		perror("malloc");
		return (NULL);
	}
	return (result);
}

char	*replace_exit_status(char *result, int *j, int *i)
{
	char	g_exit_statusc[12];

	int_to_str(g_exit_status, g_exit_statusc);
	ft_strcpy(&result[*j], g_exit_statusc);
	*j += ft_strlen(g_exit_statusc);
	*i += 2;
	return (result);
}

char	*replace_env_variable(char *command, char *result, int *j, int *i)
{
	char	*var_start;
	char	*var_end;
	char	var_name[256];
	char	*var_value;

	var_start = &command[*i + 1];
	var_end = var_start;
	while (*var_end && (ft_isalnum((int)*var_end) || *var_end == '_'))
		var_end++;
	ft_strncpy(var_name, var_start, var_end - var_start);
	var_name[var_end - var_start] = '\0';
	var_value = get_env_value(var_name);
	if (var_value)
	{
		ft_strcpy(&result[*j], var_value);
		*j += ft_strlen(var_value);
	}
	*i = var_end - command;
	return (result);
}

char	*initialize_variables(char *command, int *i, int *j,
		int *in_single_quote)
{
	char	*result;

	*i = 0;
	*j = 0;
	*in_single_quote = 0;
	result = allocate_result_buffer(command);
	if (!result)
	{
		free(result);
		return (NULL);
	}
	return (result);
}

char	*handle_dollar(char *command)
{
	char *result;
	int i;
	int j;
	int in_single_quote;

	result = initialize_variables(command, &i, &j, &in_single_quote);
	if (!result)
		return (NULL);
	while (command[i] != '\0')
	{
		if (command[i] == '\'')
		{
			in_single_quote = !in_single_quote;
			result[j++] = command[i++];
		}
		else if (command[i] == '$' && command[i + 1] == '?')
			result = replace_exit_status(result, &j, &i);
		else if (command[i] == '$' && (i == 0 || command[i - 1] != '\\')
			&& !in_single_quote)
			result = replace_env_variable(command, result, &j, &i);
		else
			result[j++] = command[i++];
	}
	result[j] = '\0';
	return (result);
}