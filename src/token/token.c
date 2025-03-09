/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 18:18:01 by oissa             #+#    #+#             */
/*   Updated: 2025/03/08 23:41:05 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	if_token_started(t_parse_cmd *parse_cmd, t_env *env_list)
{
	if (parse_cmd->token_started)
	{
		parse_cmd->buffer[parse_cmd->j] = '\0';
		parse_cmd->index_splitter = parse_cmd->i;
		if ((parse_cmd->token_was_single_quoted
				|| parse_cmd->token_was_dollar_quote)
			|| !ft_strchr(parse_cmd->buffer, '$'))
		{
			parse_cmd->cmd.args[parse_cmd->i++] = ft_strdup(parse_cmd->buffer);
			if (parse_cmd->cmd.args[parse_cmd->i - 1] == NULL)
				return (EXIT_FAILURE);
		}
		else
		{
			parse_cmd->cmd.args[parse_cmd->i++] = expand_env_variables_in_token(
					parse_cmd->buffer, env_list, parse_cmd);
			if (parse_cmd->cmd.args[parse_cmd->i - 1] == NULL)
				return (EXIT_FAILURE);
		}
		reset_parse_cmd(parse_cmd);
	}
	return (EXIT_SUCCESS);
}

int	if_token_started_three(t_parse_cmd *parse_cmd, t_env *env_list)
{
	if (!parse_cmd->token_started)
		return (EXIT_SUCCESS);
	parse_cmd->index_splitter = parse_cmd->i;
	parse_cmd->buffer[parse_cmd->j] = '\0';
	if ((parse_cmd->token_was_single_quoted
			|| parse_cmd->token_was_dollar_quote)
		&& !is_dollar_inside_quote(parse_cmd->clean_input))
		parse_cmd->cmd.args[parse_cmd->i++] = ft_strdup(parse_cmd->buffer);
	else
	{
		parse_cmd->cmd.args[parse_cmd->i++] = expand_env_variables_in_token(
				parse_cmd->buffer, env_list, parse_cmd);
		if (parse_cmd->cmd.args[parse_cmd->i - 1] == NULL)
			return (EXIT_FAILURE);
	}
	reset_parse_cmd(parse_cmd);
	return (EXIT_SUCCESS);
}

int	parse_cmd_loop(t_parse_cmd *parse_cmd, t_env *env_list)
{
	while (parse_cmd->clean_input[parse_cmd->k])
	{
		parse_cmd->c = parse_cmd->clean_input[parse_cmd->k];
		if (ft_is_operator(parse_cmd->clean_input + parse_cmd->k) == 2)
			parse_cmd->operator = parse_cmd->clean_input[++parse_cmd->k];
		if (check_condition(parse_cmd) == EXIT_SUCCESS)
			continue ;
		if (check_condition_too(parse_cmd, env_list) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		parse_cmd->operator = '\0';
	}
	return (EXIT_SUCCESS);
}

int	parse_disassemble_args(t_parse_cmd *parse_cmd, t_cmd *cmd_result,
		t_main *main)
{
	int		i;
	char	**split;

	parse_cmd->must_splitter[parse_cmd->index_splitter + 1] = (size_t)-1;
	split = smart_split(main->input);
	i = -1;
	while (++i <= parse_cmd->index_splitter)
	{
		if (parse_cmd->must_splitter[i] == 1)
		{
			if (i == 0 || ((i > 0 && (split[i - 1] && ft_strchr(split[i - 1],
								'|'))) || split[i][0] != '"'))
				parse_cmd->must_splitter[i] = 1;
			else
				parse_cmd->must_splitter[i] = 0;
		}
	}
	ft_free_split(split);
	if (disassemble_args(parse_cmd) != EXIT_SUCCESS)
	{
		free_cmd_parse(parse_cmd, cmd_result);
		return (EXIT_FAILURE);
	}
	*cmd_result = parse_cmd->cmd;
	return (EXIT_SUCCESS);
}

t_cmd	*parse_cmd(t_main *main)
{
	t_parse_cmd	parse_cmd;
	t_cmd		*cmd_result;

	cmd_result = ft_calloc(1, sizeof(t_cmd));
	if (!cmd_result)
	{
		main->exit_status = 1;
		return (NULL);
	}
	if (init_parse_cmd(&parse_cmd, main) && !free_cmd_parse(&parse_cmd,
			cmd_result))
		return (NULL);
	if ((parse_cmd_loop(&parse_cmd, main->env_list)
			|| if_token_started_three(&parse_cmd, main->env_list))
		&& !free_cmd_parse(&parse_cmd, cmd_result))
		return (NULL);
	if (parse_cmd.in_quotes && !free_cmd_parse(&parse_cmd, cmd_result))
		return (NULL);
	if (clean_parse_cmd(&parse_cmd) && !free_cmd_parse(&parse_cmd, cmd_result))
		return (NULL);
	if (parse_disassemble_args(&parse_cmd, cmd_result, main))
		return (NULL);
	free_part_parse_cmd(&parse_cmd);
	return (cmd_result);
}
