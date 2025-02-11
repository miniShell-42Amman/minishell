/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_env_value.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:07:45 by oissa             #+#    #+#             */
/*   Updated: 2025/02/11 15:07:45 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *find_env_value(t_env *env_list, const char *var_name)
{
    t_env *current;

    current = env_list;
    while (current)
    {
        if (ft_strcmp(current->key, var_name) == 0)
            return (current->value);
        current = current->next;
    }
    return ("");
}
