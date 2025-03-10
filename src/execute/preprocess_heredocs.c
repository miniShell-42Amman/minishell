/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocess_heredocs.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 23:49:00 by oissa             #+#    #+#             */
/*   Updated: 2025/03/10 02:28:09 by lalhindi         ###   ########.fr       */
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

int	check_for_cmp(t_redirections *redirections, t_execute *execute,
		t_main *main, int *num_command)
{
	if (ft_strcmp(redirections->argv[redirections->j], "<<") == 0
		&& main->tokens_list[*num_command].type != TOKEN_ARGUMENT)
	{
		signal(SIGINT, handle_heredoc_sigint);
		if (redirection_check_else_if(redirections, execute,
				main) != EXIT_SUCCESS)
			return (EXIT_FAILURE);
		if (g_signal == 130)
		{
			free_redirections(redirections);
			free_execute(execute, 0);
			return (EXIT_BREAK);
		}
	}
	else
	{
		redirections->j++;
		(*num_command)++;
	}
	return (EXIT_SUCCESS);
}

int	loop_heredoc(t_redirections *redirections, t_execute *execute, t_main *main,
		int *num_command)
{
	int	result;

	while (redirections->argv[redirections->j])
	{
		if (main->tokens_list[*num_command].value
			&& main->tokens_list[*num_command].type == TOKEN_PIPE)
			(*num_command)++;
		result = check_for_cmp(redirections, execute, main, num_command);
		if (result == EXIT_FAILURE)
			return (EXIT_FAILURE);
		else if (result == EXIT_BREAK)
			break ;
	}
	return (EXIT_SUCCESS);
}

void	init_struct(t_redirections *redirections, t_execute *execute, int i)
{
	ft_bzero(redirections, sizeof(t_redirections));
	redirections->argv = execute->commands[i];
	redirections->j = 0;
}

int	preprocess_heredocs(t_execute *execute, t_main *main)
{
	t_redirections	redirections;
	size_t			i;
	int				num_command;
	int				tmp;

	tmp = dup(STDIN_FILENO);
	if (tmp < 0)
	{
		perror("dup");
		return (EXIT_FAILURE);
	}
	i = -1;
	num_command = 0;
	while (++i < execute->num_cmds)
	{
		init_struct(&redirections, execute, i);
		if (loop_heredoc(&redirections, execute, main,
				&num_command) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (if_global_signal(&tmp) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	close(tmp);
	signal(SIGINT, handle_sigint);
	return (EXIT_SUCCESS);
}
