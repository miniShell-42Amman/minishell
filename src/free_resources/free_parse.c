/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 19:06:49 by oissa             #+#    #+#             */
/*   Updated: 2025/03/13 17:08:12 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *tokens, int token_count)
{
	int	i;

	i = 0;
	if (!tokens)
		return ;
	while (i < token_count)
	{
		if (tokens[i].value)
		{
			free(tokens[i].value);
			tokens[i].value = NULL;
		}
		i++;
	}
	free(tokens);
	tokens = NULL;
}

int	free_cmd_parse(t_parse_cmd *parse_cmd, t_cmd *cmd_result)
{
	if (parse_cmd->clean_input)
	{
		free(parse_cmd->clean_input);
		parse_cmd->clean_input = NULL;
	}
	if (parse_cmd->splitter_clean_input)
	{
		ft_free_split(parse_cmd->splitter_clean_input);
		parse_cmd->splitter_clean_input = NULL;
	}
	if (parse_cmd)
	{
		ft_free_parse_cmd(parse_cmd);
		parse_cmd = NULL;
	}
	if (cmd_result)
	{
		free_command(cmd_result);
		cmd_result = NULL;
	}
	return (EXIT_SUCCESS);
}

t_cmd	ft_free_parse_cmd(t_parse_cmd *parse_cmd)
{
	ft_dprintf(2, RED "Error404: " RESET "Unclosed quotes\n");
	*parse_cmd->exit_status = 2;
	if (parse_cmd->clean_input)
		free(parse_cmd->clean_input);
	if (parse_cmd->cmd.args)
		ft_free_split(parse_cmd->cmd.args);
	if (parse_cmd->splitter_clean_input)
		ft_free_split(parse_cmd->splitter_clean_input);
	if (parse_cmd->must_splitter)
		free(parse_cmd->must_splitter);
	if (parse_cmd->cmd.cmd)
		free(parse_cmd->cmd.cmd);
	if (parse_cmd->buffer)
		free(parse_cmd->buffer);
	if (parse_cmd->arr_has_dollar)
		free(parse_cmd->arr_has_dollar);
	parse_cmd->clean_input = NULL;
	parse_cmd->cmd.args = NULL;
	parse_cmd->splitter_clean_input = NULL;
	parse_cmd->cmd.cmd = NULL;
	parse_cmd->buffer = NULL;
	parse_cmd->arr_has_dollar = NULL;
	parse_cmd->must_splitter = NULL;
	return (parse_cmd->cmd);
}

int	clean_parse_cmd(t_parse_cmd *parse_cmd)
{
	parse_cmd->cmd.args[parse_cmd->i] = NULL;
	if (parse_cmd->cmd.args[0])
	{
		parse_cmd->cmd.cmd = ft_strdup(parse_cmd->cmd.args[0]);
		if (parse_cmd->cmd.cmd == NULL)
			return (EXIT_FAILURE);
	}
	else
		parse_cmd->cmd.cmd = NULL;
	if (parse_cmd->clean_input)
		free(parse_cmd->clean_input);
	if (parse_cmd->splitter_clean_input)
		ft_free_split(parse_cmd->splitter_clean_input);
	if (parse_cmd->arr_has_dollar)
		free(parse_cmd->arr_has_dollar);
	if (parse_cmd->buffer)
		free(parse_cmd->buffer);
	parse_cmd->clean_input = NULL;
	parse_cmd->arr_has_dollar = NULL;
	parse_cmd->buffer = NULL;
	return (EXIT_SUCCESS);
}

void	free_command(t_cmd *cmd)
{
	int	i;

	if (cmd->cmd)
	{
		free(cmd->cmd);
		cmd->cmd = NULL;
	}
	if (cmd->args)
	{
		i = -1;
		while (++i < cmd->arg_count)
		{
			if (cmd->args[i])
			{
				free(cmd->args[i]);
				cmd->args[i] = NULL;
			}
		}
		if (cmd->args)
		{
			free(cmd->args);
			cmd->args = NULL;
		}
	}
	free(cmd);
}
