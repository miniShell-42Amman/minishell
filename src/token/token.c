/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 18:18:01 by oissa             #+#    #+#             */
/*   Updated: 2025/02/16 02:37:54 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_parse_cmd(t_parse_cmd *parse_cmd, char *input)
{
	ft_bzero(parse_cmd, sizeof(t_parse_cmd));
	ft_bzero(&parse_cmd->cmd, sizeof(t_cmd));
	parse_cmd->trimmed_input = ft_strtrim(input, " \t\n");
	parse_cmd->clean_input = ft_strdup(parse_cmd->trimmed_input);
	free(parse_cmd->trimmed_input);
	if (!parse_cmd->clean_input)
		return (EXIT_FAILURE);
	parse_cmd->buffer = ft_calloc((ft_strlen(parse_cmd->clean_input) + 1),sizeof(char));
	if (!parse_cmd->buffer)
	{
		free(parse_cmd->clean_input);
		return (EXIT_FAILURE);
	}
	parse_cmd->cmd.arg_count = count_args(parse_cmd->clean_input);
	parse_cmd->cmd.args = ft_calloc((parse_cmd->cmd.arg_count + 1),
									sizeof(char *));
	if (!parse_cmd->cmd.args)
	{
		free(parse_cmd->clean_input);
		free(parse_cmd->buffer);
		return (EXIT_FAILURE);
	}
	parse_cmd->cmd.args[parse_cmd->cmd.arg_count] = NULL;
	return (EXIT_SUCCESS);
}

int	if_token_started(t_parse_cmd *parse_cmd, t_env *env_list)
{
	if (parse_cmd->token_started)
	{
		parse_cmd->buffer[parse_cmd->j] = '\0';
		if ((parse_cmd->token_was_single_quoted
			|| parse_cmd->token_was_dollar_quote) && !parse_cmd->has_dollar)
		{
			parse_cmd->cmd.args[parse_cmd->i++] = ft_strdup(parse_cmd->buffer);
			if (parse_cmd->cmd.args[parse_cmd->i - 1] == NULL)
				return (EXIT_FAILURE);
		}
	 	else
			parse_cmd->cmd.args[parse_cmd->i++]
                        = expand_env_variables_in_token(parse_cmd->buffer,
					env_list, &parse_cmd->has_dollar, parse_cmd);
		parse_cmd->j = 0;
		parse_cmd->token_started = false;
		parse_cmd->token_quote_type = '\0';
		parse_cmd->token_was_single_quoted = false;
		parse_cmd->token_was_dollar_quote = false;
	}
	return (EXIT_SUCCESS);
}

int	if_token_started_three(t_parse_cmd *parse_cmd, t_env *env_list)
{
	if (parse_cmd->token_started)
	{
		parse_cmd->buffer[parse_cmd->j] = '\0';
		if ((parse_cmd->token_was_single_quoted
			|| parse_cmd->token_was_dollar_quote) && !parse_cmd->has_dollar)
		{
			parse_cmd->cmd.args[parse_cmd->i++] = ft_strdup(parse_cmd->buffer);
			if (parse_cmd->cmd.args[parse_cmd->i - 1] == NULL)
			return (EXIT_FAILURE);
		}
		else
		{
				parse_cmd->cmd.args[parse_cmd->i++]
						= expand_env_variables_in_token(parse_cmd->buffer,
			env_list, &parse_cmd->has_dollar, parse_cmd);
		}
	}
	return (EXIT_SUCCESS);
}

int	parse_cmd_loop(t_parse_cmd *parse_cmd, t_env *env_list)
{
	while (parse_cmd->clean_input[parse_cmd->k])
	{
		parse_cmd->c = parse_cmd->clean_input[parse_cmd->k];
		if (ft_is_operator(parse_cmd->clean_input + parse_cmd->k) == 2)
			parse_cmd->operator= parse_cmd->clean_input[++parse_cmd->k];
		if (check_condition(parse_cmd) == EXIT_SUCCESS)
			continue ;
		if (check_condition_too(parse_cmd, env_list) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		parse_cmd->operator= '\0';
	}
	return (EXIT_SUCCESS);
}

t_cmd	*parse_cmd(char *input, t_env *env_list)
{
	t_parse_cmd	parse_cmd;
	t_cmd		*cmd_result;

	cmd_result = ft_calloc(1, sizeof(t_cmd));
	if ((!cmd_result || init_parse_cmd(&parse_cmd, input))
		&& !free_cmd_parse(&parse_cmd, cmd_result))
		return (NULL);
	if ((parse_cmd_loop(&parse_cmd, env_list)
			|| if_token_started_three(&parse_cmd, env_list))
		&& !free_cmd_parse(&parse_cmd, cmd_result))
		return (NULL);
	if (parse_cmd.in_quotes && !free_cmd_parse(&parse_cmd, cmd_result))
		return (NULL);
	if (clean_parse_cmd(&parse_cmd) && !free_cmd_parse(&parse_cmd, cmd_result))
		return (NULL);
	*cmd_result = parse_cmd.cmd;
	return (cmd_result);
}
