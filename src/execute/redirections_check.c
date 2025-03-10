/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 02:11:02 by lalhindi          #+#    #+#             */
/*   Updated: 2025/03/10 02:24:57 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shift_redirections_argv(t_redirections *redirections)
{
	while (redirections->argv[redirections->k + 2])
	{
		redirections->argv[redirections->k] = redirections->argv[redirections->k
			+ 2];
		redirections->k++;
	}
}

int	redirection_check(t_redirections *redirections, t_main *main,
		t_execute *execute)
{
	t_fd_flags	fd_flags;

	redirections->op = redirections->argv[redirections->j];
	redirections->target = redirections->argv[redirections->j + 1];
	if (!redirections->target && ft_dprintf(STDERR_FILENO,
			"Erorr404: syntax error near token `%s'\n", redirections->op))
	{
		free_redirections(redirections);
		exit(EXIT_FAILURE);
	}
	choose_flags_fd(redirections, &fd_flags, main, execute);
	if (dup2(fd_flags.fd, fd_flags.std_fd) < 0)
	{
		perror("Erorr404: dup2");
		free_redirections(redirections);
		exit(EXIT_FAILURE);
	}
	close(fd_flags.fd);
	redirections->k = redirections->j;
	shift_redirections_argv(redirections);
	redirections->argv[redirections->k] = NULL;
	return (EXIT_SUCCESS);
}

int	redirection_check_else_if_loop(t_redirections *redirections,
		t_here_document *here_doc, t_execute *execute, t_main *main)
{
	(void)execute;
	while (1)
	{
		here_doc->line = readline("> ");
		if (handle_heredoc_signal(here_doc, redirections)
			|| handle_heredoc_eof(here_doc) || handle_heredoc_target(here_doc))
			break ;
		if (append_heredoc_line(here_doc, redirections, main) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		free(here_doc->line);
	}
	if (here_doc->current_doc)
	{
		redirections->heredoc_all = append_str(redirections->heredoc_all,
				&redirections->heredoc_total_size, here_doc->current_doc);
		free_here_doc(here_doc);
	}
	return (EXIT_SUCCESS);
}

int	redirection_check_else_if(t_redirections *redirections, t_execute *execute,
		t_main *main)
{
	t_here_document	here_doc;

	ft_bzero(&here_doc, sizeof(t_here_document));
	here_doc.target = redirections->argv[redirections->j + 1];
	if (!here_doc.target)
	{
		ft_dprintf(STDERR_FILENO, "Erorr404: syntax error near token `<<'\n");
		free_redirections(redirections);
		free_here_doc(&here_doc);
		*redirections->exit_status = 2;
		redirections = NULL;
		return (EXIT_FAILURE);
	}
	signal(SIGINT, handle_heredoc_sigint);
	if (redirection_check_else_if_loop(redirections, &here_doc, execute,
			main) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	cleanup_redirections_argv(redirections);
	free_here_doc(&here_doc);
	return (EXIT_SUCCESS);
}
