/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 21:49:48 by oissa             #+#    #+#             */
/*   Updated: 2025/03/16 03:48:02 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_and_free(t_execute *execute, t_main *main)
{
	free_execute(execute, 0);
	free_resources(main, 1);
	exit(*execute->exit_status);
}

void	execute_part(t_execute *execute, t_main *main)
{
	if (!execute->cmd_path)
	{
		if (*execute->exit_status == 126)
			ft_dprintf(2, "Error404: %s: Is a directory\n",
				execute->commands[execute->i][0]);
		else
			ft_dprintf(2, "Error404: %s: command not found\n",
				execute->commands[execute->i][0]);
		exit_and_free(execute, main);
	}
	if (execve(execute->cmd_path, execute->commands[execute->i], execute->envp))
	{
		errno_massage(execute);
		exit_and_free(execute, main);
	}
}

void	execute_command(t_execute *execute, t_main *main)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (execute->i != 0 && dup2(execute->pipe_fds[(execute->i - 1) * 2],
			STDIN_FILENO) < 0)
		exit_and_free(execute, main);
	if (execute->i != execute->num_cmds - 1
		&& dup2(execute->pipe_fds[execute->i * 2 + 1], STDOUT_FILENO) < 0)
		exit_and_free(execute, main);
	execute->j = -1;
	while (++execute->j < 2 * execute->num_pipes)
		close(execute->pipe_fds[execute->j]);
	handle_redirections(execute, main);
	if (!is_commands(execute, 2))
	{
		execute_builtin(execute, main);
		exit_and_free(execute, main);
	}
	execute->cmd_path = resolve_command_path(execute->commands[execute->i][0],
			execute, main);
	execute_part(execute, main);
}

int	check_condition_execute(t_execute *execute, t_main *main, int *check_pipes)
{
	if (preprocess_heredocs(execute, main) != EXIT_SUCCESS)
	{
		main->exit_status = 130;
		free_execute(execute, 0);
		free_resources(main, 0);
		return (EXIT_FAILURE);
	}
	if (create_pipes(execute, check_pipes))
	{
		main->exit_status = 1;
		free_execute(execute, 0);
		free_resources(main, 0);
		return (EXIT_FAILURE);
	}
	if (fork_and_execute(execute, main))
	{
		main->exit_status = 1;
		free_execute(execute, 0);
		free_resources(main, 0);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	start_execution(t_main *main)
{
	t_execute	execute;
	int			check_pipes;

	check_pipes = 0;
	ft_bzero(&execute, sizeof(t_execute));
	execute.envp = convert_env_to_list(main->env_list);
	execute.exit_status = &main->exit_status;
	*execute.exit_status = 0;
	execute.num_cmds = calculate_number_operations(main->tokens_list,
			main->cmd->arg_count) + 1;
	execute.num_pipes = execute.num_cmds - 1;
	if (fill_commands(main->tokens_list, main->cmd->arg_count, &execute)
		|| !execute.cmd_args)
	{
		main->exit_status = 1;
		free_execute(&execute, 0);
		free_resources(main, 0);
		return ;
	}
	if (check_condition_execute(&execute, main, &check_pipes) == EXIT_FAILURE)
		return ;
	close_pipes_and_wait(&execute);
	free_execute(&execute, 0);
	return ;
}
