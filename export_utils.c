/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 10:18:59 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/13 10:23:08 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern char	**environ;;
void	skip_whitespace(char *input, int *i)
{
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
}

int	extract_varname(char *input, char *varname, int *i)
{
	unsigned long	j;

	j = 0;
	while (input[*i] && input[*i] != '=' && input[*i] != ' '
		&& input[*i] != '\t' && j < sizeof(varname) - 1)
		varname[j++] = input[(*i)++];
	varname[j] = '\0';
	if (input[*i] != '=')
	{
		printf("error: export missing '=' after variable name\n");
		return (0);
	}
	(*i)++;
	return (1);
}

void	extract_val(char *input, char *value, int *i)
{
	unsigned long	j;

	j = 0;
	while (input[*i] && input[*i] != ' ' && input[*i] != '\t'
		&& j < sizeof(value) - 1)
		value[j++] = input[(*i)++];
	value[j] = '\0';
}

int	get_env_size(void)
{
	int	env_size;

	env_size = 0;
	while (environ[env_size] != NULL)
		env_size++;
	return (env_size);
}
