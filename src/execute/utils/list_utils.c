/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 22:23:35 by lalhindi          #+#    #+#             */
/*   Updated: 2025/03/10 00:19:27 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_list_size(t_env *env_list)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env_list;
	while (current && current->key)
	{
		count++;
		current = current->next;
	}
	return (count);
}

int	allocate_env_entry(char ***envp, t_env *current, int i)
{
	size_t	envp_len;

	envp_len = 0;
	if (current->has_value && current->value)
		envp_len = ft_strlen(current->key) + ft_strlen(current->value) + 2;
	else if (current->key)
		envp_len = ft_strlen(current->key) + 2;
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
		ft_strlcat((*envp)[i], "=", envp_len);
	return (EXIT_SUCCESS);
}

int	fill_env_list(char ***envp, t_env *env_list, int envp_count)
{
	int		i;
	t_env	*current;

	i = 0;
	current = env_list;
	while (i < envp_count && current != NULL)
	{
		if (allocate_env_entry(envp, current, i) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		current = current->next;
		i++;
	}
	(*envp)[i] = NULL;
	return (EXIT_SUCCESS);
}

char	**convert_env_to_list(t_env *env_list)
{
	int		env_count;
	char	**envp;

	env_count = ft_list_size(env_list);
	envp = ft_calloc(env_count + 1, sizeof(char *));
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
