/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 02:04:56 by lalhindi          #+#    #+#             */
/*   Updated: 2025/03/10 02:09:19 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_heredoc_signal(t_here_document *here_doc,
		t_redirections *redirections)
{
	if (g_signal == 130)
	{
		free_here_doc(here_doc);
		free_redirections(redirections);
		return (1);
	}
	return (0);
}

int	handle_heredoc_eof(t_here_document *here_doc)
{
	if (!here_doc->line)
	{
		ft_dprintf(STDERR_FILENO,
			"Erorr404: warning: here-document delimited by EOF\n");
		free_here_doc(here_doc);
		return (1);
	}
	return (0);
}

int	handle_heredoc_target(t_here_document *here_doc)
{
	if (ft_strcmp(here_doc->line, here_doc->target) == 0)
	{
		free_here_doc(here_doc);
		return (1);
	}
	return (0);
}

int	append_heredoc_line(t_here_document *here_doc, t_redirections *redirections,
		t_main *main)
{
	here_doc->current_doc = append_str(here_doc->current_doc,
			&here_doc->current_size, here_doc->line);
	here_doc->current_doc = append_str(here_doc->current_doc,
			&here_doc->current_size, "\n");
	if (here_doc->current_doc == NULL)
	{
		perror("Erorr404: realloc");
		free_redirections(redirections);
		free_resources(main, 1);
		free_here_doc(here_doc);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	cleanup_redirections_argv(t_redirections *redirections)
{
	if (redirections->argv && redirections->argv[redirections->j])
	{
		free(redirections->argv[redirections->j]);
		redirections->argv[redirections->j] = NULL;
	}
	redirections->k = redirections->j;
	while (redirections->argv[redirections->k + 2])
	{
		redirections->argv[redirections->k] = redirections->argv[redirections->k
			+ 2];
		redirections->k++;
	}
	redirections->argv[redirections->k] = NULL;
}
