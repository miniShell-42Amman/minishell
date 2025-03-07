/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parse_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 22:46:31 by oissa             #+#    #+#             */
/*   Updated: 2025/03/07 22:51:55 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_parse_cmd_too(t_parse_cmd *parse_cmd)
{
	parse_cmd->buffer = ft_calloc((ft_strlen(parse_cmd->clean_input) + 1),
			sizeof(char));
	if (!parse_cmd->buffer)
	{
		free(parse_cmd->clean_input);
		return (EXIT_FAILURE);
	}
	parse_cmd->cmd.arg_count = 0;
	if (parse_cmd->splitter_clean_input)
	{
		while (parse_cmd->splitter_clean_input[parse_cmd->cmd.arg_count])
			parse_cmd->cmd.arg_count++;
	}
	parse_cmd->cmd.args = ft_calloc((parse_cmd->cmd.arg_count + 1),
			sizeof(char *));
	if (!parse_cmd->cmd.args)
	{
		free(parse_cmd->clean_input);
		free(parse_cmd->buffer);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	init_parse_cmd(t_parse_cmd *parse_cmd, t_main *main)
{
	ft_bzero(parse_cmd, sizeof(t_parse_cmd));
	ft_bzero(&parse_cmd->cmd, sizeof(t_cmd));
	parse_cmd->program_name = main->program_name;
	parse_cmd->trimmed_input = ft_strtrim(main->input, " \t\n");
	parse_cmd->clean_input = ft_strdup(parse_cmd->trimmed_input);
	parse_cmd->splitter_clean_input = smart_split(parse_cmd->clean_input);
	parse_cmd->must_splitter = ft_calloc(ft_array_size(parse_cmd->splitter_clean_input)
			+ 1, sizeof(size_t));
	if (!parse_cmd->splitter_clean_input)
	{
		free(parse_cmd->clean_input);
		free(parse_cmd->trimmed_input);
		return (EXIT_FAILURE);
	}
	parse_cmd->exit_status = &(main->exit_status);
	free(parse_cmd->trimmed_input);
	parse_cmd->trimmed_input = NULL;
	if (!parse_cmd->clean_input)
		return (EXIT_FAILURE);
	if (init_parse_cmd_too(parse_cmd) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	parse_cmd->cmd.args[parse_cmd->cmd.arg_count] = NULL;
	return (EXIT_SUCCESS);
}
