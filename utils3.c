/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 14:51:27 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/22 15:50:47 by tjehaes          ###   ########.fr       */
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

char	*replace_exit_status(t_env *env, char *result, int *j, int *i)
{
	char	exit_statusc[12];

	int_to_str(env->exit_status, exit_statusc);
	ft_strcpy(&result[*j], exit_statusc);
	*j += ft_strlen(exit_statusc);
	*i += 2;
	return (result);
}

char	*replace_env_variable(t_env *env, char *command, char *result,
		int indexes[2])
{
	char	*var_start;
	char	*var_end;
	char	var_name[256];
	char	*var_value;

	var_start = &command[indexes[1] + 1];
	var_end = var_start;
	while (*var_end && (ft_isalnum((int)*var_end) || *var_end == '_'))
		var_end++;
	ft_strncpy(var_name, var_start, var_end - var_start);
	var_name[var_end - var_start] = '\0';
	var_value = get_env_value(env, var_name);
	if (var_value)
	{
		ft_strcpy(&result[indexes[0]], var_value);
		indexes[0] += ft_strlen(var_value);
		free(var_value);
	}
	indexes[1] = var_end - command;
	return (result);
}

char	*initialize_variables(char *command, int indexes[2],
		int *in_single_quote)
{
	char	*result;

	indexes[0] = 0;
	indexes[1] = 0;
	*in_single_quote = 0;
	result = allocate_result_buffer(command);
	if (!result)
	{
		free(result);
		return (NULL);
	}
	return (result);
}

char	*handle_dollar(t_env *env, char *command)
{
	char	*result;
	int		indexes[2];
	int		in_single_quote;

	result = initialize_variables(command, indexes, &in_single_quote);
	if (!result)
		return (NULL);
	while (command[indexes[1]] != '\0')
	{
		if (command[indexes[1]] == '\'')
		{
			in_single_quote = !in_single_quote;
			result[indexes[0]++] = command[indexes[1]++];
		}
		else if (command[indexes[1]] == '$' && command[indexes[1] + 1] == '?')
			result = replace_exit_status(env, result, &indexes[0], &indexes[1]);
		else if (command[indexes[1]] == '$' && (indexes[1] == 0
				|| command[indexes[1] - 1] != '\\') && !in_single_quote)
			result = replace_env_variable(env, command, result, indexes);
		else
			result[indexes[0]++] = command[indexes[1]++];
	}
	result[indexes[0]] = '\0';
	return (result);
}
