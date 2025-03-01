/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 22:49:19 by oissa             #+#    #+#             */
/*   Updated: 2025/02/27 06:28:17 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int unset(char **args, t_env **env_list)
{
    t_env *current;
    t_env *prev;

    if (!args[1])
        return 0;
    current = *env_list;
    prev = NULL;
    while (current)
    {
        if (ft_strcmp(current->key, args[1]) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                *env_list = current->next;
            free(current->key);
            free(current->value);
            free(current);
            return 0;
        }
        prev = current;
        current = current->next;
    }
    return 0;
}