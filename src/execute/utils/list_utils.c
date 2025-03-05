#include "minishell.h"

int ft_list_size(t_env *env_list)
{
    int count = 0;
    t_env *current = env_list;
    while (current && current->key)
    {
        count++;
        current = current->next;
    }
    return (count);
}

int fill_env_list(char ***envp, t_env *env_list, int envp_count)
{
    int i = 0;
    t_env *current = env_list;

    while (i < envp_count && current != NULL)
    {
        size_t envp_len = 0;
        if (current->has_value && current->value)
            envp_len = ft_strlen(current->key) + ft_strlen(current->value) + 2;
        else if(current->key)
            envp_len = ft_strlen(current->key) + 2;

        // (*envp)[i] = malloc(envp_len);
        (*envp)[i] = ft_calloc(envp_len, sizeof(char));
        if (!(*envp)[i])
        {
            ft_free_split(*envp);
            return (EXIT_FAILURE);
        }
        
        ft_strlcpy((*envp)[i], current->key, envp_len);
        if (current->has_value && current->value)
        {
            ft_strlcat((*envp)[i], "=", envp_len);
            ft_strlcat((*envp)[i], current->value, envp_len);
        }
        else
        {
            ft_strlcat((*envp)[i], "=", envp_len);
        }
        
        current = current->next;
        i++;
    }
    (*envp)[i] = NULL;
    return (EXIT_SUCCESS);
}

// Fixed environment conversion
char **convert_env_to_list(t_env *env_list)
{
    int env_count = ft_list_size(env_list);
    char **envp = ft_calloc(env_count + 1, sizeof(char *));
    
    if (!envp)
    {
        ft_free_split(envp);
        perror("minishell: malloc error");
        return (NULL);
    }
    
    if (fill_env_list(&envp, env_list, env_count))
    {
        ft_free_split(envp);
        return (NULL);
    }
    return (envp);
}