/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 00:19:20 by oissa             #+#    #+#             */
/*   Updated: 2025/02/02 00:19:20 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_command(t_cmd *cmd)
{
    int i;
    
    if (cmd->cmd)
        free(cmd->cmd);  
    if (cmd->args)
    {
        i = 0;
        while (i < cmd->arg_count)
        {
            free(cmd->args[i]);
            i++;
        }
        free(cmd->args);
    }
}