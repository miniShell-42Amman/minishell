/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_execute.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:53:58 by oissa             #+#    #+#             */
/*   Updated: 2025/03/03 02:56:58 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_execute_too(t_execute *execute)
{
    if (execute->cmd_args)
    {
        free(execute->cmd_args);
        execute->cmd_args = NULL;
    }
    if (execute->pipe_fds)
    {
        free(execute->pipe_fds);
        execute->pipe_fds = NULL;
    }
    if (execute->pids)
    {
        free(execute->pids);
        execute->pids = NULL;
    }
    if (execute->cmd_path)
    {
        free(execute->cmd_path);
        execute->cmd_path = NULL;
    }
}

void free_execute(t_execute *execute, int flage)
{
    execute->i = 0;
    if (execute->envp)
    {
        ft_free_split(execute->envp);
        execute->envp = NULL;
    }
    if (execute->commands)
    {
        while (execute->i < execute->num_cmds)
        {
            if (execute->commands[execute->i])
            {
                ft_free_split(execute->commands[execute->i]);
                execute->commands[execute->i] = NULL;
            }
            execute->i++;
        }
        free(execute->commands);
        execute->commands = NULL;
    }
    free_execute_too(execute);
    if (flage && execute->env_list != NULL)
    {
        free_env_list(execute->env_list);
        execute->env_list = NULL;
    }
}
