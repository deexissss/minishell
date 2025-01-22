/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 09:12:15 by tjehaes           #+#    #+#             */
/*   Updated: 2025/01/13 11:00:38 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


static void	check_var(char *input, int *i)
{
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
	if (input[*i] == '\0')
	{
		printf("error: unset needs a variable name\n");
		g_env.exit_status = 1;
	}
}

static void	extract_varname_u(char *input, char *varname, int *i)
{
	unsigned long	j;

	j = 0;
	while (input[*i] && input[*i] != ' ' && input[*i] != '\t'
		&& j < sizeof(varname) - 1)
		varname[j++] = input[(*i)++];
	varname[j] = '\0';
}

/*static void	remove_var_from_env(char *varname)
{
	unsigned long	j;
	unsigned long	j;
	unsigned long	len;
	char			*env_var;
	unsigned long	i;
	int				varname_len;

	j = 0;
	while (environ[j] != NULL)
	{
		if (ft_strncmp(environ[j], varname, ft_strlen(varname)) == 0
			&& environ[j][ft_strlen(varname)] == '=')
		{
			if (environ[j] != NULL && environ[j] != varname)
				free(environ[j]);
			while (environ[j + 1] != NULL)
			{
				environ[j] = environ[j + 1];
				j++;
			}
			environ[j] = NULL;
			return ;
		}
		j++;
	}
}*/
/*static void	remove_var_from_env(char *varname)
{
	j = 0;
	len = strlen(varname);
	while (environ[j] != NULL)
	{
		env_var = environ[j];
		if (strncasecmp(env_var, varname, len) == 0 && env_var[len] == '=')
		{
			// Vérifie si la variable a été allouée dynamiquement
			if (env_var != varname)
			{
				// Libère la mémoire allouée pour la variable d'environnement
				free(env_var);
			}
			while (environ[j + 1] != NULL)
			{
				environ[j] = environ[j + 1];
				j++;
			}
			environ[j] = NULL;
			return ;
		}
		j++;
	}
}*/
static void	remove_var_from_env(char *varname)
{
    int	i;
    int varname_len;

	i = 0;
	varname_len = ft_strlen(varname);
	while (g_env.variables[i] != NULL)
	{
		if (ft_strncmp(g_env.variables[i], varname, varname_len) == 0
			&& g_env.variables[i][varname_len] == '=')
		{
			free(g_env.variables[i]);
			break ;
		}
		i++;
	}
	if (g_env.variables[i] == NULL)
		return ;
	while (g_env.variables[i + 1] != NULL)
	{
		g_env.variables[i] = g_env.variables[i + 1];
		i++;
	}
	g_env.variables[i] = NULL;
	g_env.size--;
}

void	execute_unset(char *input)
{
	int		i;
	char	varname[256];

	i = 6;
	g_env.exit_status = 0;
	while (input[i] != '\0')
	{
		check_var(input, &i);
		if (g_env.exit_status != 0)
			return ;
		extract_varname_u(input, varname, &i);
		remove_var_from_env(varname);
		while (input[i] == ' ' || input[i] == '\t')
			i++;
	}
}
