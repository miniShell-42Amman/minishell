/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 00:55:30 by oissa             #+#    #+#             */
/*   Updated: 2025/03/06 00:56:49 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void increment_shell_level(t_env **env_list)
{
	char *shell_level_str;
	int shell_level_int;
	char *shell_level_str_new;

	shell_level_str_new = ft_strdup("SHLVL");
	shell_level_str = get_env_var(*env_list, "SHLVL");
	if (shell_level_str)
	{
		shell_level_int = ft_atoi(shell_level_str);
		shell_level_int++;
	}
	else
	{
		shell_level_int = 1;
	}
	update_env(env_list, shell_level_str_new, ft_itoa(shell_level_int));
}

char *search_command(char *search)
{
	char *path = getenv("PATH");
	char **paths = ft_split(path, ':');
	char *command;
	char *tmp;
	int i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		command = ft_strjoin(tmp, search);
		if (access(command, F_OK) == 0)
		{
			ft_free_split(paths);
			free(tmp);
			return command;
		}
		free(tmp);
		free(command);
		i++;
	}
	ft_free_split(paths);
	return NULL;
}
