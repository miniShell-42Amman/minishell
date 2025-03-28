/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 22:21:13 by lalhindi          #+#    #+#             */
/*   Updated: 2025/03/16 03:49:35 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirections(t_redirections *redirections)
{
	if (redirections->heredoc_all)
	{
		free(redirections->heredoc_all);
		redirections->heredoc_all = NULL;
	}
	if (redirections->current_doc)
	{
		free(redirections->current_doc);
		redirections->current_doc = NULL;
	}
	if (redirections->line)
	{
		free(redirections->line);
		redirections->line = NULL;
	}
}

void	free_here_doc(t_here_document *here_doc, int free_target)
{
	if (here_doc->line)
	{
		free(here_doc->line);
		here_doc->line = NULL;
	}
	if (free_target && here_doc->current_doc)
	{
		free(here_doc->current_doc);
		here_doc->current_doc = NULL;
	}
	if (here_doc->target)
	{
		free(here_doc->target);
		here_doc->target = NULL;
	}
}
