/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 17:24:44 by oissa             #+#    #+#             */
/*   Updated: 2025/02/05 17:25:49 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void add_node(t_env **head, t_env *new_node)
{
    t_env *current;

    if (*head == NULL)
    {
        *head = new_node;
        return;
    }
    current = *head;
    while (current->next != NULL)
        current = current->next;
    current->next = new_node;
}

t_env *save_env(char **env)
{
    int i = 0;
    t_env *env_list = NULL;
    t_env *new_node;

    while (env[i])
    {
        new_node = malloc(sizeof(t_env));
        if (!new_node)
            return NULL;
        int len_for_key = 0;
        new_node->in_init = false;
        while (env[i][len_for_key]/* && env[i][len_for_key] != '='*/)
        {
            if (env[i][len_for_key] == '=')
            {
                new_node->in_init = true;
                break;
            }
            len_for_key++;
        }
        new_node->key = ft_substr(env[i], 0, len_for_key);
        if (new_node->in_init == true)
            new_node->value = ft_strdup(env[i] + len_for_key + 1);
        else
            new_node->value = NULL;
        new_node->next = NULL;

        if (new_node->key == NULL || new_node->value == NULL)
        {            
            free(new_node->key);
            free(new_node->value);
            free(new_node);
            return NULL;
        }
        add_node(&env_list, new_node);
        i++;
    }
    return env_list;
}
