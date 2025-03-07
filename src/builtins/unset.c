/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 22:25:54 by lalhindi          #+#    #+#             */
/*   Updated: 2025/03/08 00:36:03 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_env_name(char *str)
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

void	unset_loop(t_unset *unset, char **args, t_env **env_list)
{
	while (unset->current)
	{
		if (ft_strcmp(unset->current->key, args[unset->i]) == 0)
		{
			if (unset->prev)
				unset->prev->next = unset->current->next;
			else
				*env_list = unset->current->next;
			free(unset->current->key);
			unset->current->key = NULL;
			free(unset->current->value);
			unset->current->value = NULL;
			free(unset->current);
			unset->current = NULL;
			break ;
		}
		unset->prev = unset->current;
		unset->current = unset->current->next;
	}
}

int	unset(char **args, t_env **env_list)
{
	t_unset	unset;

	unset.i = 1;
	ft_bzero(&unset, sizeof(t_unset));
	if (!args[1])
		return (0);
	while (args[unset.i])
	{
		if (!is_valid_env_name(args[unset.i]))
		{
			ft_dprintf(2, "unset: '%s': not a valid identifier\n",
				args[unset.i]);
			unset.exit_status = 1;
			unset.i++;
			continue ;
		}
		unset.current = *env_list;
		unset.prev = NULL;
		unset_loop(&unset, args, env_list);
		unset.i++;
	}
	return (unset.exit_status);
}
