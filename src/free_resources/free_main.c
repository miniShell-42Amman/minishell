/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:32:19 by oissa             #+#    #+#             */
/*   Updated: 2025/02/15 17:32:19 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_resources(t_main *main, int flag)
{
	if (!main)
		return ;
	if (main->input)
	{
		free(main->input);
		main->input = NULL;
	}
	if (main->env_list && flag)
	{
		free_env_list(main->env_list);
		main->env_list = NULL;
	}
	if (main->tokens_list && main->cmd)
	{
		free_tokens(main->tokens_list, main->cmd->arg_count);
		main->tokens_list = NULL;
	}
	if (main->cmd)
	{
		free_command(main->cmd);
		main->cmd = NULL;
	}
}
