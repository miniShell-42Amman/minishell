/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 02:11:02 by lalhindi          #+#    #+#             */
/*   Updated: 2025/03/16 03:49:00 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redirection_check(t_redirections *redirections, t_main *main,
		t_execute *execute, int i)
{
	t_fd_flags	fd_flags;

	redirections->op = redirections->argv[i];
	redirections->target = redirections->argv[i + 1];
	if (!redirections->target && ft_dprintf(STDERR_FILENO,
			"Erorr404: syntax error near token `%s'\n", redirections->op))
	{
		free_execute(execute, 0);
		free_resources(main, 1);
		*redirections->exit_status = 2;
		exit(EXIT_FAILURE);
	}
	choose_flags_fd(redirections, &fd_flags, main, execute);
	if (fd_flags.fd != -1)
	{
		if (dup2(fd_flags.fd, fd_flags.std_fd) == -1)
			ft_perror_free_exit("dup2 error", execute, main, redirections);
		close(fd_flags.fd);
	}
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
		free_here_doc(here_doc, 0);
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
		free_here_doc(&here_doc, 0);
		*redirections->exit_status = 2;
		redirections = NULL;
		return (EXIT_FAILURE);
	}
	signal(SIGINT, handle_heredoc_sigint);
	if (redirection_check_else_if_loop(redirections, &here_doc, execute,
			main) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	cleanup_redirections_argv(redirections);
	free_here_doc(&here_doc, 1);
	return (EXIT_SUCCESS);
}
