/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_and_fork.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 23:26:58 by oissa             #+#    #+#             */
/*   Updated: 2025/03/12 22:17:53 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_pipes(t_execute *execute, int *check)
{
	execute->pipe_fds = malloc(2 * execute->num_pipes * sizeof(int));
	if (!execute->pipe_fds)
	{
		*check = 1;
		return (EXIT_FAILURE);
	}
	execute->i = 0;
	while ((int)execute->i < execute->num_pipes)
	{
		if (pipe(execute->pipe_fds + execute->i * 2) < 0)
		{
			*check = 1;
			return (EXIT_FAILURE);
		}
		execute->i++;
	}
	return (EXIT_SUCCESS);
}

void	free_and_exit_status(t_execute *execute, t_main *main)
{
	free_execute(execute, 1);
	main->exit_status = 1;
}

int	start_cmd_main_proses(t_execute *execute, t_main *main)
{
	if (handle_builtins(execute, main) == 10)
		return (EXIT_FAILURE);
	execute->i++;
	return (EXIT_CONTINUE);
}

int	fork_and_execute(t_execute *execute, t_main *main)
{
	execute->pids = ft_calloc(execute->num_cmds, sizeof(pid_t));
	if (!execute->pids)
		return (EXIT_FAILURE);
	execute->i = 0;
	while (execute->i < execute->num_cmds)
	{
		if (execute->num_cmds == 1 && is_commands(execute, 1) == EXIT_SUCCESS)
		{
			if (start_cmd_main_proses(execute, main) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			continue ;
		}
		execute->pid = fork();
		if (execute->pid < 0)
		{
			free_and_exit_status(execute, main);
			return (EXIT_FAILURE);
		}
		if (execute->pid == 0)
			execute_command(execute, main);
		else
			execute->pids[execute->i] = execute->pid;
		execute->i++;
	}
	return (EXIT_SUCCESS);
}

void	close_pipes_and_wait(t_execute *execute)
{
	int		status;
	pid_t	pid;
	pid_t	last_pid;

	if (!execute->pids)
		return ;
	last_pid = execute->pids[execute->num_cmds - 1];
	execute->i = 0;
	while (execute->i < (size_t)(2 * execute->num_pipes))
		close(execute->pipe_fds[execute->i++]);
	pid = wait(&status);
	while (pid != -1)
	{
		if (pid == last_pid)
		{
			if ((status & 0xFF) == 0)
				*execute->exit_status = (status >> 8) & 0xFF;
			else
				*execute->exit_status = 128 + (status & 0x7F);
		}
		pid = wait(&status);
	}
}
