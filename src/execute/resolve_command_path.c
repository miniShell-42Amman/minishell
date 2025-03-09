/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_command_path.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 22:41:44 by oissa             #+#    #+#             */
/*   Updated: 2025/03/09 23:24:47 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	git_data_for_resolve_cmd(t_resolve_command_path *resolve_cmd,
								char *command,
								t_main *main,
								t_execute *execute)
{
	resolve_cmd->full_path = malloc(ft_strlen(resolve_cmd->path_token)
			+ ft_strlen(command) + 2);
	if (!resolve_cmd->full_path)
	{
		free(resolve_cmd->path_copy);
		perror("malloc");
		free_execute(execute, 0);
		free_resources(main, 1);
		exit(EXIT_FAILURE);
	}
	ft_strlcpy(resolve_cmd->full_path, resolve_cmd->path_token,
		ft_strlen(resolve_cmd->path_token) + 1);
	ft_strlcat(resolve_cmd->full_path, "/", ft_strlen(resolve_cmd->path_token)
		+ 2);
	ft_strlcat(resolve_cmd->full_path, command,
		ft_strlen(resolve_cmd->path_token) + ft_strlen(command) + 2);
}

char	*handle_valid_path(t_resolve_command_path *resolve_cmd,
						t_execute *execute)
{
	if (access(resolve_cmd->full_path, X_OK) == 0)
	{
		if (is_directory(resolve_cmd->full_path))
		{
			free(resolve_cmd->path_copy);
			*execute->exit_status = 126;
			return (resolve_cmd->full_path);
		}
		free(resolve_cmd->path_copy);
		return (resolve_cmd->full_path);
	}
	else
	{
		free(resolve_cmd->path_copy);
		*execute->exit_status = 126;
		return (NULL);
	}
}

char	*resolve_command_path(char *command, t_execute *execute, t_main *main)
{
	t_resolve_command_path	resolve_cmd;

	ft_bzero(&resolve_cmd, sizeof(t_resolve_command_path));
	if (check_command_resolve(command, execute, main) == EXIT_FAILURE)
		return (NULL);
	if (command[0] == '/' || command[0] == '.')
		return (handle_absolute_or_relative_path(command, execute));
	resolve_cmd.path = find_env_value(main->env_list, "PATH");
	if (!resolve_cmd.path)
		return (NULL);
	dup_and_check(&resolve_cmd, execute, main);
	resolve_cmd.path_token = ft_strtok(resolve_cmd.path_copy, ":");
	while (resolve_cmd.path_token)
	{
		git_data_for_resolve_cmd(&resolve_cmd, command, main, execute);
		if (access(resolve_cmd.full_path, F_OK) == 0)
			return (handle_valid_path(&resolve_cmd, execute));
		free(resolve_cmd.full_path);
		resolve_cmd.path_token = ft_strtok(NULL, ":");
	}
	free(resolve_cmd.path_copy);
	return (*execute->exit_status = 127, NULL);
}
