/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 22:30:22 by oissa             #+#    #+#             */
/*   Updated: 2025/02/09 23:29:02 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_node(t_env **node)
{
	*node = (t_env *)malloc(sizeof(t_env));
	if (!(*node))
		return (EXIT_FAILURE);
	(*node)->key = NULL;
	(*node)->value = NULL;
	(*node)->next = NULL;
	(*node)->has_value = false;
	return (EXIT_SUCCESS);
}

int	add_node_to_env(t_env **head, t_env *new_node)
{
	t_env	*current;

	if (create_node(&new_node) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (!*head)
		*head = new_node;
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
	return (EXIT_SUCCESS);
}

int	init_values(t_env *new_node, char **object)
{
	new_node->key = ft_strdup(object[0]);
	if (!new_node->key)
	{
		free(new_node);
		return (EXIT_FAILURE);
	}
	new_node->value = ft_strdup(object[1]);
	if (!new_node->value)
	{
		free(new_node->key);
		free(new_node);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

t_env	*clone_env(char **env)
{
	int		i;
	char	*object[2];
	t_env	*new_node;
	t_env	*head;
	t_env	*current;

	i = -1;
	head = NULL;
	while (env[++i])
	{
		if (create_node(&new_node) == EXIT_FAILURE || splitter_object(object,
				env[i], &new_node->has_value) == EXIT_FAILURE
			|| init_values(new_node, object) == EXIT_FAILURE)
		{
			free_object(object, head);
			return (NULL);
		}
		if (!head)
			head = new_node;
		else
			current->next = new_node;
		current = new_node;
		free_object(object, NULL);
	}
	return (head);
}

int	splitter_object(char **object, char *str, bool *has_value)
{
	int		len_key;
	bool	has_value_from_str;

	has_value_from_str = false;
	len_key = ft_strlen_sep(str, '=', &has_value_from_str);
	object[0] = ft_substr(str, 0, len_key);
	if (!object[0])
		return (EXIT_FAILURE);
	*has_value = has_value_from_str;
	if (has_value_from_str)
		object[1] = ft_substr(str, len_key + 1, ft_strlen(str) - len_key);
	else
		object[1] = ft_strdup("");
	if (!object[1])
	{
		free(object[0]);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
