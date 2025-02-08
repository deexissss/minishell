/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg >    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 12:53:38 by tjehaes           #+#    #+#             */
/*   Updated: 2025/02/07 15:17:28 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	simple_dollar(t_env *env, char *inpt)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = ft_strlen(inpt);
	while (*inpt == '$')
	{
		count++;
		inpt++;
	}
	if (count == i)
	{
		ft_printf("error: command not found\n");
		env->exit_status = 127;
		return (1);
	}
	return (0);
}

int	handle_quote(t_env *env, char *inpt)
{
	int	i;
	int	countsimple;
	int	countdouble;

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
		env->exit_status = 130;
		return (1);
	}
	return (0);
}

void	process_input(t_env *env, char *input)
{
	if (check_empty_functions(input) == 0 || simple_dollar(env, input) == 1
		|| check_multiple_pipe(env, input) == 1 || check_quote_empty(input) == 1
		|| check_check(input) == 1)
		free(input);
	else if (handle_quote(env, input) == 0)
		execute_commands(env, input);
	else
		free(input);
}

sig_atomic_t	g_sigint_received = 0;

/*int	main(void)
{
	t_env	env;
	char	*inpt;
	int		saved_stdin;
	int		saved_stdout;

	init_shell(&env, &saved_stdin, &saved_stdout);
	while (1)
	{
		inpt = readline("Minishell$ ");
		if (!inpt)
			break ;
		if (g_sigint_received)
		{
			env.exit_status = 130;
			g_sigint_received = 0;
		}
		if (*inpt)
			add_history(inpt);
		process_input(&env, inpt);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		rl_replace_line("", 0);
		rl_redisplay();
	}
	close(saved_stdin);
	close(saved_stdout);
	free_struct(&env);
	return (0);
}*/

void	process_input_loop(t_env *env, int saved_stdin, int saved_stdout)
{
	char	*inpt;

	while (1)
	{
		inpt = readline("Minishell$ ");
		if (!inpt)
			break ;
		if (g_sigint_received)
		{
			env->exit_status = 130;
			g_sigint_received = 0;
		}
		if (*inpt && !is_line_empty(inpt))
			add_history(inpt);
		process_input(env, inpt);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	main(void)
{
	t_env	env;
	int		saved_stdin;
	int		saved_stdout;

	init_shell(&env, &saved_stdin, &saved_stdout);
	process_input_loop(&env, saved_stdin, saved_stdout);
	close(saved_stdin);
	close(saved_stdout);
	free_struct(&env);
	return (0);
}
