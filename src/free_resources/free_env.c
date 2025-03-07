/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 22:41:47 by oissa             #+#    #+#             */
/*   Updated: 2025/02/10 23:38:38 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_list(t_env *env_list)
{
	t_env	*head;
	if(!env_list)
		return ;
	while (env_list)
	{
		head = env_list;
		env_list = env_list->next;
		if (head->key)
		{
			free(head->key);
			head->key = NULL;
		}
		if (head->value)
		{
			free(head->value);
			head->value = NULL;
		}
		free(head);
		head = NULL;
	}
	free(env_list);
}

void	free_object(char **object, t_env *head)
{
	if (object[0])
	{
		free(object[0]);
		object[0] = NULL;
	}
	if (object[1])
	{
		free(object[1]);
		object[1] = NULL;
	}
	if (head)
	{
		free_env_list(head);
		head = NULL;
	}
}
