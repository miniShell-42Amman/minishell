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

void  free_env(t_env *env_list)
{
    t_env *current = env_list;
    t_env *next;

    while (current)
    {
        next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }
}
void free_env_list(t_env *env_list)
{
    t_env *temp;

    while (env_list)
    {
        temp = env_list;
        env_list = env_list->next;
        free(temp->key);
        free(temp->value);
        free(temp);
    }
}