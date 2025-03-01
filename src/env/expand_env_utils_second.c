/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_utils_second.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 22:02:51 by lalhindi          #+#    #+#             */
/*   Updated: 2025/03/01 02:20:23 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_existing_env(t_env *tmp, char *v)
{
	if (tmp && v)
	{
		free(tmp->value);
		tmp->value = NULL;
		tmp->value = ft_strdup(v);
		tmp->has_value = 1;
	}
}

char	*get_env_var(t_env *env, const char *key)
{
	while (env)
	{
		if (!ft_strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	add_new_env(t_env **env, char *k, char *v)
{
	t_env	*new_node;

	if (v)
		new_node = new_node_env(ft_strdup(k), ft_strdup(v));
	else
		new_node = new_node_env(ft_strdup(k), NULL);
	if (!new_node)
		return ;
	if (v)
		new_node->has_value = 1;
	else
		new_node->has_value = 0;
	add_new_node(env, new_node);
}

void	update_env(t_env **env, char *k, char *v)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp && ft_strcmp(tmp->key, k))
		tmp = tmp->next;
	if (tmp)
		update_existing_env(tmp, v);
	else
		add_new_env(env, k, v);
	free(k);
	k = NULL;
	free(v);
	v = NULL;
}

void	ft_sort_arr(char **arr, int size)
{
	int		i;
	int		j;
	char	*tmp;

	i = -1;
	while (++i < size)
	{
		j = -1;
		while (++j < size - i - 1)
		{
			if (ft_strcmp(arr[j], arr[j + 1]))
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
		}
	}
}
