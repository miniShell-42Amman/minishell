/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_length.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:45:32 by oissa             #+#    #+#             */
/*   Updated: 2025/03/09 22:15:05 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_if_dollar_quote(t_expand_env *expand, const char *token)
{
	if (expand->parse_cmd->splitter_clean_input
		&& expand->parse_cmd->splitter_clean_input[
			expand->parse_cmd->index_splitter]
		&& is_dolloar_quote(token) == is_dolloar_quote(
			expand->parse_cmd->splitter_clean_input[
				expand->parse_cmd->index_splitter])
		&& is_dolloar_quote(token) > 0)
	{
		calculate_dollar_array(expand->parse_cmd);
	}
}

void	free_dollar_array(t_parse_cmd *parse_cmd)
{
	if (parse_cmd->arr_has_dollar)
	{
		free(parse_cmd->arr_has_dollar);
		parse_cmd->arr_has_dollar = NULL;
	}
}

int	if_check_for_loop(t_expand_env *expand, const char **token, size_t *len)
{
	if (**token == '$'
		&& !is_string_inside_single(expand->parse_cmd->splitter_clean_input[
				expand->parse_cmd->index_splitter])
		&& ((*(*token + 1)) != ' ' && (*(*token + 1)) != '\0'))
	{
		(*token)++;
		*len += handle_var_length(token, expand);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

size_t	calculate_length(t_expand_env *expand)
{
	size_t		len;
	bool		squote;
	bool		dquote;
	const char	*token;

	len = 0;
	squote = false;
	dquote = false;
	token = expand->token;
	check_if_dollar_quote(expand, token);
	while (*token)
	{
		update_quote_state(*token, &squote, &dquote);
		if (if_check_for_loop(expand, &token, &len) == EXIT_SUCCESS)
			continue ;
		len++;
		token++;
	}
	free_dollar_array(expand->parse_cmd);
	return (len);
}
