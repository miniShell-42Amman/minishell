/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   full_and_init_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 23:47:32 by oissa             #+#    #+#             */
/*   Updated: 2025/03/09 23:47:49 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_command(t_token *token, size_t token_count, t_execute *execute)
{
	execute->commands = ft_calloc(execute->num_cmds, sizeof(char **));
	execute->cmd_args = ft_calloc(execute->num_cmds, sizeof(size_t));
	if (!execute->commands || !execute->cmd_args)
		return (EXIT_FAILURE);
	execute->i = -1;
	while (++execute->i < token_count)
	{
		if (token[execute->i].type == TOKEN_PIPE)
			execute->cmd_index++;
		else
			execute->cmd_args[execute->cmd_index]++;
	}
	execute->i = -1;
	while (++execute->i < execute->num_cmds)
	{
		execute->commands[execute->i] = ft_calloc(execute->cmd_args[execute->i]
				+ 1, sizeof(char *));
		if (!execute->commands[execute->i])
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	fill_commands(t_token *token, size_t token_count, t_execute *execute)
{
	if (init_command(token, token_count, execute))
		return (EXIT_FAILURE);
	execute->i = -1;
	execute->cmd_index = 0;
	execute->arg_index = 0;
	while (++execute->i < token_count)
	{
		if (token[execute->i].type == TOKEN_PIPE)
		{
			execute->cmd_index++;
			execute->arg_index = 0;
		}
		else
		{
			execute->commands[execute->cmd_index][execute->arg_index]
				= ft_strdup(token[execute->i].value);
			if (!execute->commands[execute->cmd_index][execute->arg_index])
				return (EXIT_FAILURE);
			execute->arg_index++;
			free(token[execute->i].value);
			token[execute->i].value = NULL;
		}
	}
	return (EXIT_SUCCESS);
}
