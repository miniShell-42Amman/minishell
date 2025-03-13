/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess_heredocs_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 02:06:52 by oissa             #+#    #+#             */
/*   Updated: 2025/03/13 17:10:25 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	if_global_signal(int *tmp)
{
	if (g_signal == 130)
	{
		dup2(*tmp, STDIN_FILENO);
		close(*tmp);
		signal(SIGINT, handle_sigint);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	get_num_tokens(char **command)
{
	int	count;

	count = 0;
	if (command)
	{
		while (command[count])
			count++;
	}
	return (count);
}

int	*compute_start_indices(t_execute *execute)
{
	int		*start_indices;
	int		current_start;
	int		num_tokens;
	size_t	i;

	start_indices = malloc(execute->num_cmds * sizeof(int));
	if (!start_indices)
		return (NULL);
	current_start = 0;
	i = -1;
	while (++i < execute->num_cmds)
	{
		start_indices[i] = current_start;
		num_tokens = get_num_tokens(execute->commands[i]);
		current_start += num_tokens;
		if (i < execute->num_cmds - 1)
			current_start += 1;
	}
	return (start_indices);
}

int	check_for_cmp(t_redirections *redirections, t_execute *execute,
		t_main *main)
{
	int	token_index;

	token_index = redirections->start_index + redirections->j;
	if (ft_strcmp(redirections->argv[redirections->j], "<<") == 0
		&& main->tokens_list[token_index].type != TOKEN_ARGUMENT)
	{
		signal(SIGINT, handle_heredoc_sigint);
		if (redirection_check_else_if(redirections, execute, main)
			!= EXIT_SUCCESS)
			return (EXIT_FAILURE);
		if (g_signal == 130)
		{
			free_redirections(redirections);
			free_execute(execute, 0);
			return (EXIT_BREAK);
		}
	}
	else
		redirections->j++;
	return (EXIT_SUCCESS);
}
