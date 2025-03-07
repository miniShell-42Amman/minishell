/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 22:25:54 by lalhindi          #+#    #+#             */
/*   Updated: 2025/03/07 04:57:45 by oissa            ###   ########.fr       */
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
    // t_env *temp = *env_list;
    // while (temp)
    // {
    //     ft_printf("before key: %s, value: %s\n", temp->key, temp->value);
    //     temp = temp->next;
    // }
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
                        **env_list = *current->next;
                current->has_value = 0;   
                free(current->key);
                free(current->value);
                free(current);
                current = NULL;
                break;
            }
            prev = current;
            current = current->next;
        }
        i++;
    }
    
    // t_env *temps = *env_list;
    // while (temps)
    // {
    //     ft_printf("Afteerererere key: %s, value: %s\n", temps->key, temps->value);
    //     temps = temps->next;
    // }
    
    return (exit_status);
}