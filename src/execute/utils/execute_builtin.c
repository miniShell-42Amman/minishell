/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 23:28:23 by oissa             #+#    #+#             */
/*   Updated: 2025/03/10 00:14:56 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_builtin(t_execute *execute, t_main *main)
{
	*execute->exit_status = 0;
	if (ft_strcmp(execute->commands[execute->i][0], "echo") == 0)
		echo(execute->commands[execute->i]);
	else if (ft_strcmp(execute->commands[execute->i][0], "exit") == 0)
		*execute->exit_status = ft_exit(execute->commands[execute->i]);
	else if (ft_strcmp(execute->commands[execute->i][0], "env") == 0)
		env(execute->envp);
	else if (ft_strcmp(execute->commands[execute->i][0], "pwd") == 0)
		pwd();
	else if (ft_strcmp(execute->commands[execute->i][0], "cd") == 0)
		*execute->exit_status = cd(execute->commands[execute->i],
				execute->cmd_args[execute->i], &main->env_list);
	else if (ft_strcmp(execute->commands[execute->i][0], "export") == 0)
		*execute->exit_status = export(execute->commands[execute->i],
				execute->cmd_args[execute->i], &main->env_list);
	else if (ft_strcmp(execute->commands[execute->i][0], "unset") == 0)
		*execute->exit_status = unset(execute->commands[execute->i],
				&main->env_list);
}

int	handle_builtins(t_execute *execute, t_main *main)
{
	if (ft_strcmp(execute->commands[execute->i][0], "cd") == 0)
		*execute->exit_status = cd(execute->commands[execute->i],
				execute->cmd_args[execute->i], &main->env_list);
	else if (ft_strcmp(execute->commands[execute->i][0], "export") == 0)
		*execute->exit_status = export(execute->commands[execute->i],
				execute->cmd_args[execute->i], &main->env_list);
	else if (ft_strcmp(execute->commands[execute->i][0], "unset") == 0)
		*execute->exit_status = unset(execute->commands[execute->i],
				&main->env_list);
	else if (ft_strcmp(execute->commands[execute->i][0], "exit") == 0)
	{
		*execute->exit_status = ft_exit(execute->commands[execute->i]);
		free_execute(execute, 0);
		free_resources(main, 1);
		exit(*execute->exit_status);
	}
	return (EXIT_SUCCESS);
}
