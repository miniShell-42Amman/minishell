/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_cmd_path_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 23:24:29 by oissa             #+#    #+#             */
/*   Updated: 2025/03/09 23:25:19 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_command_resolve(char *command, t_execute *execute, t_main *main)
{
	if (!command || !main->env_list)
	{
		free_execute(execute, 0);
		free_resources(main, 1);
		*execute->exit_status = 127;
		exit(*execute->exit_status);
	}
	if (command[0] == '\0')
	{
		*execute->exit_status = 127;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

char	*handle_absolute_or_relative_path(char *command, t_execute *execute)
{
	if (access(command, F_OK) == 0)
	{
		if (access(command, X_OK) == 0)
		{
			if (is_directory(command))
			{
				*execute->exit_status = 126;
				return (ft_strdup(command));
			}
			return (ft_strdup(command));
		}
		else
		{
			*execute->exit_status = 126;
			return (NULL);
		}
	}
	*execute->exit_status = 127;
	return (NULL);
}

void	dup_and_check(t_resolve_command_path *resolve_cmd, t_execute *execute,
		t_main *main)
{
	resolve_cmd->path_copy = ft_strdup(resolve_cmd->path);
	if (!resolve_cmd->path_copy)
	{
		perror("strdup");
		free_execute(execute, 0);
		free_resources(main, 1);
		exit(EXIT_FAILURE);
	}
}
