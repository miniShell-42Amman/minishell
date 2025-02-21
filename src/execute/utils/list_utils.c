/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 19:50:24 by oissa             #+#    #+#             */
/*   Updated: 2025/02/21 20:19:08 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_list_size(t_env *env_list)
{
    if (env_list == NULL)
        return (EXIT_SUCCESS);
    else if (env_list->next == NULL)
        return (EXIT_FAILURE);
    else
        return (1 + ft_list_size(env_list->next));
}
int fill_env_list(char ***envp, t_env *env_list, int envp_count)
{
    int i;

    i = 0;
    while (i < envp_count)
    {
        size_t envp_len;

        envp_len = ft_strlen(env_list->key) + ft_strlen(env_list->value) + 2;
        (*envp)[i] = malloc(envp_len);
        if (!(*envp)[i]) 
        {
            perror("malloc");
            ft_free_split(*envp);
            return (EXIT_FAILURE);
        }
        ft_strlcpy((*envp)[i], env_list->key, envp_len);
        ft_strlcat((*envp)[i], "=", envp_len);
        if (env_list->value)
            ft_strlcat((*envp)[i], env_list->value, envp_len);
        i++;
        env_list = env_list->next;
    }
    (*envp)[envp_count] = NULL;
    return (EXIT_SUCCESS);
}


char **convert_env_to_list(t_env *env_list)
{
    int env_count = 0;
    t_env *current = env_list;
    env_count = ft_list_size(current);
    char **envp = malloc((env_count + 1) * sizeof(char *));
    if (!envp)
    {
        perror("malloc");
        return (NULL);    
    }
    if (fill_env_list(&envp, env_list, env_count))
        return (NULL);
    return (envp);
}
