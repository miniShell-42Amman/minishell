/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 18:18:01 by oissa             #+#    #+#             */
/*   Updated: 2025/03/07 23:18:37 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int if_token_started(t_parse_cmd *parse_cmd, t_env *env_list)
{
	if (parse_cmd->token_started)
	{
		parse_cmd->buffer[parse_cmd->j] = '\0';
		parse_cmd->index_splitter = parse_cmd->i;
		if ((parse_cmd->token_was_single_quoted || parse_cmd->token_was_dollar_quote) || !ft_strchr(parse_cmd->buffer, '$'))
		{
			parse_cmd->cmd.args[parse_cmd->i++] = ft_strdup(parse_cmd->buffer);
			if (parse_cmd->cmd.args[parse_cmd->i - 1] == NULL)
				return (EXIT_FAILURE);
		}
		else
			parse_cmd->cmd.args[parse_cmd->i++] = expand_env_variables_in_token(parse_cmd->buffer,
																				env_list, parse_cmd);
		parse_cmd->j = 0;
		parse_cmd->token_started = false;
		parse_cmd->token_quote_type = '\0';
		parse_cmd->token_was_single_quoted = false;
		parse_cmd->token_was_dollar_quote = false;
	}
	return (EXIT_SUCCESS);
}
int is_dollar_inside_quote(const char *str)
{
	int i;
	bool squote;
	bool dquote;

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

int if_token_started_three(t_parse_cmd *parse_cmd, t_env *env_list)
{
	if (!parse_cmd->token_started)
		return (EXIT_SUCCESS);
	parse_cmd->index_splitter = parse_cmd->i;
	parse_cmd->buffer[parse_cmd->j] = '\0';
	if ((parse_cmd->token_was_single_quoted || parse_cmd->token_was_dollar_quote) && !is_dollar_inside_quote(parse_cmd->clean_input))
		parse_cmd->cmd.args[parse_cmd->i++] = ft_strdup(parse_cmd->buffer);
	else
		parse_cmd->cmd.args[parse_cmd->i++] = expand_env_variables_in_token(parse_cmd->buffer,
																			env_list, parse_cmd);
	parse_cmd->j = 0;
	parse_cmd->token_started = false;
	parse_cmd->token_quote_type = '\0';
	parse_cmd->token_was_single_quoted = false;
	parse_cmd->token_was_dollar_quote = false;
	return (EXIT_SUCCESS);
}

int parse_cmd_loop(t_parse_cmd *parse_cmd, t_env *env_list)
{
	while (parse_cmd->clean_input[parse_cmd->k])
	{
		parse_cmd->c = parse_cmd->clean_input[parse_cmd->k];
		if (ft_is_operator(parse_cmd->clean_input + parse_cmd->k) == 2)
			parse_cmd->operator= parse_cmd->clean_input[++parse_cmd->k];
		if (check_condition(parse_cmd) == EXIT_SUCCESS)
			continue;
		if (check_condition_too(parse_cmd, env_list) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		parse_cmd->operator= '\0';
	}
	return (EXIT_SUCCESS);
}

int parse_disassemble_args(t_parse_cmd *parse_cmd, t_cmd *cmd_result)
{
	parse_cmd->must_splitter[parse_cmd->index_splitter + 1] = (size_t)-1;
	if (disassemble_args(parse_cmd) != EXIT_SUCCESS)
	{
		free_cmd_parse(parse_cmd, cmd_result);
		return (EXIT_FAILURE);
	}
	*cmd_result = parse_cmd->cmd;
	free(parse_cmd->buffer);
	free(parse_cmd->clean_input);
	free(parse_cmd->must_splitter);
	return (EXIT_SUCCESS);
}

t_cmd *parse_cmd(t_main *main)
{
	t_parse_cmd parse_cmd;
	t_cmd *cmd_result;

	cmd_result = ft_calloc(1, sizeof(t_cmd));
	if (!cmd_result)
	{
		main->exit_status = 1;
		return (NULL);
	}
	if (init_parse_cmd(&parse_cmd, main) && !free_cmd_parse(&parse_cmd,
															cmd_result))
		return (NULL);
	if ((parse_cmd_loop(&parse_cmd, main->env_list) || if_token_started_three(&parse_cmd, main->env_list)) && !free_cmd_parse(&parse_cmd, cmd_result))
		return (NULL);
	if (parse_cmd.in_quotes && !free_cmd_parse(&parse_cmd, cmd_result))
		return (NULL);
	if (clean_parse_cmd(&parse_cmd) && !free_cmd_parse(&parse_cmd, cmd_result))
		return (NULL);
	if (parse_disassemble_args(&parse_cmd, cmd_result) == EXIT_FAILURE)
		return (NULL);
	return (cmd_result);
}
