/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 10:18:59 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/22 15:50:54 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	extract_varname(char *input, char **varname, int *i)
{
	unsigned long	j;
	unsigned long	len;

	len = ft_strlen(input);
	j = 0;
	*varname = malloc(len + 1);
	if (*varname == NULL)
	{
		perror("malloc");
		return (0);
	}
	while (input[*i] && input[*i] != '=' && input[*i] != ' '
		&& input[*i] != '\t')
		(*varname)[j++] = input[(*i)++];
	(*varname)[j] = '\0';
	if (input[*i] != '=')
	{
		printf("export: invalid identifier\n");
		return (0);
	}
	(*i)++;
	return (1);
}

void	extract_val(char *input, char **value, int *i)
{
	unsigned long	j;
	unsigned long	len;

	len = ft_strlen(input);
	j = 0;
	*value = malloc(len + 1);
	if (*value == NULL)
	{
		perror("malloc");
		return ;
	}
	while (input[*i] && input[*i] != ' ' && input[*i] != '\t')
		(*value)[j++] = input[(*i)++];
	(*value)[j] = '\0';
}

/*int	get_env_size(void)
{
	int	env_size;

	env_size = 0;
	while (g_env.variables[env_size] != NULL)
		env_size++;
	return (env_size);
}*/

void	free_directories(char *directories[], int count)
{
	while (--count >= 0)
		free(directories[count]);
}

void	verify_path_order(char *path)
{
	char	*token;
	char	*directories[256];
	int		i;

	i = 0;
	if (path == NULL)
	{
		perror("export");
		return ;
	}
	token = ft_strtok(path, ":");
	while (token != NULL)
	{
		directories[i] = malloc(ft_strlen(token) + 1);
		if (directories[i] == NULL)
		{
			perror("malloc");
			free_directories(directories, i);
			return ;
		}
		ft_strcpy(directories[i], token);
		token = ft_strtok(NULL, ":");
		i++;
	}
	free_directories(directories, i);
}
