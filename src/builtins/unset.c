/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 22:25:54 by lalhindi          #+#    #+#             */
/*   Updated: 2025/03/07 14:40:50 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int is_valid_env_name(char *str)
{
    if (!str || !*str || ft_isdigit(*str))
        return (0);

    while (*str)
    {
        if (!ft_isalnum(*str) && *str != '_')
            return (0);
        str++;
    }
    return (1);
}

int unset(char **args, t_env **env_list)
{
    t_env *current;
    t_env *prev;
    int i = 1;
    int exit_status = 0;

    if (!args[1])
        return (0);
    while (args[i])
    {
        if (!is_valid_env_name(args[i]))
        {
            ft_dprintf(2, "unset: '%s': not a valid identifier\n", args[i]);
            exit_status = 1;
            i++;
            continue;
        }

        current = *env_list;
        prev = NULL;
        while (current)
        {
            if (ft_strcmp(current->key, args[i]) == 0)
            {
                if (prev)
                    prev->next = current->next;
                else
                    *env_list = current->next;
                free(current->key);
                current->key = NULL;
                free(current->value);
                current->value = NULL;
                free(current);
                current = NULL;
                break;
            }
            prev = current;
            current = current->next;
        }
        i++;
    }
    t_env *tmp = *env_list;
    for (int i = 0; tmp; i++)
    {
        ft_printf("unset: %d: %s=%s\n", i, tmp->key, tmp->value);
        tmp = tmp->next;
    }
    return (exit_status);
}
