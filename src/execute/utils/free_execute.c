/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_execute.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:53:58 by oissa             #+#    #+#             */
/*   Updated: 2025/02/21 21:06:59 by oissa            ###   ########.fr       */
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

void free_execute(t_execute *execute)
{
    execute->i = 0;
    if (execute->envp)
        ft_free_split(execute->envp);
    if (execute->commands)
    {
        while (execute->i < execute->num_cmds)
        {
            if (execute->commands[execute->i])
                ft_free_split(execute->commands[execute->i]);
            execute->i++;
        }
        free(execute->commands);
    }
    free_execute_too(execute);
}
