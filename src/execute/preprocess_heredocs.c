/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess_heredocs.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 23:49:00 by oissa             #+#    #+#             */
/*   Updated: 2025/03/16 03:44:54 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	loop_heredoc(t_redirections *redirections, t_execute *execute, t_main *main)
{
	int	result;

	while (redirections->argv[redirections->j])
	{
		result = check_for_cmp(redirections, execute, main);
		if (result == EXIT_FAILURE)
			return (EXIT_FAILURE);
		else if (result == EXIT_BREAK)
			break ;
	}
	return (EXIT_SUCCESS);
}

void	init_redirections(t_redirections *redirections, t_execute *execute,
		int i, int start_index)
{
	ft_bzero(redirections, sizeof(t_redirections));
	redirections->argv = execute->commands[i];
	redirections->j = 0;
	redirections->start_index = start_index;
}

int	free_and_close(int **start_indices, int *tmp)
{
	free(*start_indices);
	close(*tmp);
	return (EXIT_FAILURE);
}

int	preprocess_heredocs(t_execute *execute, t_main *main)
{
	t_redirections	redirections;
	int				*start_indices;
	int				tmp;
	size_t			i;

	tmp = dup(STDIN_FILENO);
	if (tmp < 0)
		return (perror("dup"), EXIT_FAILURE);
	start_indices = compute_start_indices(execute);
	if (!start_indices)
		return (close(tmp), EXIT_FAILURE);
	i = -1;
	while (++i < execute->num_cmds)
	{
		init_redirections(&redirections, execute, i, start_indices[i]);
		if (loop_heredoc(&redirections, execute, main)
			&& free_and_close(&start_indices, &tmp))
			return (EXIT_FAILURE);
		if (if_global_signal(&tmp) && free_and_close(&start_indices, &tmp))
			return (EXIT_FAILURE);
	}
	free(start_indices);
	close(tmp);
	signal(SIGINT, handle_sigint);
	if_redirections_heredoc_all(&redirections, main, execute);
	return (EXIT_SUCCESS);
}
