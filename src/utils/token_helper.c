/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:14:28 by lalhindi          #+#    #+#             */
/*   Updated: 2025/03/08 23:34:09 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_dollar_inside_quote(const char *str)
{
	int		i;
	bool	squote;
	bool	dquote;

	i = 0;
	squote = false;
	dquote = false;
	while (str[i])
	{
		if (str[i] == '\'' && !dquote)
			squote = !squote;
		else if (str[i] == '\"' && !squote)
			dquote = !dquote;
		else if (str[i] == '$' && !squote)
			return (1);
		i++;
	}
	return (0);
}

void	free_part_parse_cmd(t_parse_cmd *parse_cmd)
{
	free(parse_cmd->buffer);
	free(parse_cmd->clean_input);
	free(parse_cmd->must_splitter);
}

void	reset_parse_cmd(t_parse_cmd *parse_cmd)
{
	parse_cmd->j = 0;
	parse_cmd->token_started = false;
	parse_cmd->token_quote_type = '\0';
	parse_cmd->token_was_single_quoted = false;
	parse_cmd->token_was_dollar_quote = false;
}
