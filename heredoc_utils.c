/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjehaes <tjehaes@student.42luxembourg      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 14:45:19 by tjehaes           #+#    #+#             */
/*   Updated: 2025/02/07 14:45:20 by tjehaes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_process_heredoc(const char *delimiter, int pipefd[2], t_env *env)
{
	close(pipefd[0]);
	read_and_write_heredoc(delimiter, pipefd, env);
}
