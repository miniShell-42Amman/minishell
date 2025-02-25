/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ossa <oissssa@student.42amman.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 19:05:40 by oissa             #+#    #+#             */
/*   Updated: 2025/02/15 23:40:380 by oiss   ssa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_have_operator(t_parse_cmd *parse_cmd)
{

	if (parse_cmd->c == '|')
		parse_cmd->cmd.args[parse_cmd->i++] = ft_strdup("|");
	else if (parse_cmd->operator== '>')
		parse_cmd->cmd.args[parse_cmd->i++] = ft_strdup(">>");
	else if (parse_cmd->operator== '<')
		parse_cmd->cmd.args[parse_cmd->i++] = ft_strdup("<<");
	else if (parse_cmd->c == '<')
		parse_cmd->cmd.args[parse_cmd->i++] = ft_strdup("<");
	else if (parse_cmd->c == '>')
		parse_cmd->cmd.args[parse_cmd->i++] = ft_strdup(">");
	if (parse_cmd->cmd.args[parse_cmd->i - 1] == NULL)
		return (EXIT_FAILURE);
	parse_cmd->k++;
	return (EXIT_SUCCESS);
}

int check_condition_too(t_parse_cmd *parse_cmd, t_env *env_list)
{
	if (!parse_cmd->in_quotes && (parse_cmd->c == ' ' || parse_cmd->c == '|' || parse_cmd->c == '<' || parse_cmd->c == '>' || parse_cmd->operator == '<' || parse_cmd->operator == '>'))
	{
		if (if_token_started(parse_cmd, env_list) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (ft_have_operator(parse_cmd) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	else
	{
		if (parse_cmd->j >= (int)(ft_strlen(parse_cmd->clean_input) + 1))
			return (EXIT_FAILURE);

		parse_cmd->buffer[parse_cmd->j++] = parse_cmd->c;
		if (parse_cmd->operator)
			parse_cmd->buffer[parse_cmd->j++] = parse_cmd->operator;
		parse_cmd->token_started = true;
		parse_cmd->k++;
	}
	return (EXIT_SUCCESS);
}

int check_condition(t_parse_cmd *parse_cmd)
{
	if (!parse_cmd->token_started && parse_cmd->c == '$' && parse_cmd->clean_input[parse_cmd->k + 1] == '"')
	{
		parse_cmd->token_was_dollar_quote = true;
		parse_cmd->k++;
		return (EXIT_SUCCESS);
	}
	if (ft_check_parse_cmd(parse_cmd) == EXIT_SUCCESS)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int ft_check_parse_cmd(t_parse_cmd *parse_cmd)
{
	if ((parse_cmd->c == '\'' || parse_cmd->c == '"') && parse_cmd->token_quote_type == '\0')
	{
		parse_cmd->token_quote_type = parse_cmd->c;
		parse_cmd->in_quotes = true;
		parse_cmd->token_started = true;
		if (parse_cmd->c == '\'')
			parse_cmd->token_was_single_quoted = true;
		parse_cmd->k++;
		return (EXIT_SUCCESS);
	}
	else if (parse_cmd->c == parse_cmd->token_quote_type && parse_cmd->in_quotes)
	{
		parse_cmd->in_quotes = false;
		parse_cmd->token_quote_type = '\0';
		parse_cmd->k++;
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

static int print_syntax_error(char *token, int *status)
{
    ft_printf("syntax error near unexpected token `%s'\n", token);
    *status = 2;
    return (EXIT_FAILURE);
}

int is_duplicate_operator_series(t_token *t, int count, int *status)
{
	int i;

	i = -1;
	while (++i < count - 1)
	{
		if (t[i].type >= 2 && t[i + 1].type >= 2)
		{
			if (t[i].type == 6 && t[i + 1].type == 6 )

				return (print_syntax_error("|", status));
			else if (t[i].type != 6)
				return (print_syntax_error(t[i + 1].value, status));
		}
		if (t[i].type == 6 && (i == 0 || i == count - 1))
			return (print_syntax_error("|", status));
		if (t[i].type == 6 && t[i + 1].type == 6)
			return (print_syntax_error("|", status));
	}
	if(t[i].type == 6)
		return (print_syntax_error("|", status));
	return (EXIT_SUCCESS);
}
