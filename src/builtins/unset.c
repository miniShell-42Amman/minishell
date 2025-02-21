/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 22:49:19 by oissa             #+#    #+#             */
/*   Updated: 2025/02/21 17:19:24 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    delete_env_var(char *var, t_env *list_env)
{
    t_env *tmp;
    t_env *prev;

    tmp = list_env;
    prev = NULL;
    while (tmp)
    {
        if (!ft_strcmp(tmp->key, var))
        {
            if (prev)
                prev->next = tmp->next;
            else
                list_env = tmp->next;
            free(tmp->key);
            free(tmp->value);
            free(tmp);
            return ;
        }
        prev = tmp;
        tmp = tmp->next;
    }
}

void    unset(char **args, t_env *env_list)
{
    int i;

    i = 1;
    while (args[i])
    {
        if (ft_strchr(args[i], '='))
        {
            ft_putstr_fd("minishell: unset: `", 2);
            ft_putstr_fd(args[i], 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
        }
        else
            delete_env_var(args[i], env_list);
        i++;
    }
}
