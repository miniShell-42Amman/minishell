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

	while (env_list)
	{
		head = env_list;
		env_list = env_list->next;
		if (head->key)
			free(head->key);
		if (head->value)
			free(head->value);
		free(head);
	}
	free(env_list);
}

void	free_object(char **object, t_env *head)
{
	if (object[0])
		free(object[0]);
	if (object[1])
		free(object[1]);
	if (head)
		free_env_list(head);
}
