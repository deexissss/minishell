/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:36:11 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/22 15:07:31 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_cd_error(t_env *env, int error_type)
{
	if (error_type == 1)
		printf("error: need a space between function and argument\n");
	else if (error_type == 2)
		printf("error: cd needs a directory path\n");
	env->exit_status = 1;
}

char	*extract_path(char *inpt, int *index)
{
	char	*path;
	int		i;
	int		j;

	i = *index;
	j = 0;
	path = malloc(sizeof(char) * 256);
	if (!path)
		return (NULL);
	while (inpt[i] && inpt[i] != ' ' && inpt[i] != '\t' && j < 255)
		path[j++] = inpt[i++];
	path[j] = '\0';
	*index = i;
	return (path);
}

void	execute_cd(t_env *env, char *inpt)
{
	int		i;
	char	*path;

	i = 2;
	env->exit_status = 0;
	while (inpt[i] == ' ' || inpt[i] == '\t')
		i++;
	if (i == 2 && inpt[i++] != '\0')
	{
		handle_cd_error(env, 1);
		return ;
	}
	path = extract_path(inpt, &i);
	if (!path || path[0] == '\0')
	{
		free(path);
		handle_cd_error(env, 2);
		return ;
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		env->exit_status = 1;
	}
	free(path);
}
