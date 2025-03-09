/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils_second.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 21:55:28 by lalhindi          #+#    #+#             */
/*   Updated: 2025/03/09 21:58:19 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_empty_first_token(char **tokens_list, int *token_count,
		t_main *main)
{
	int	original_arg_count;
	int	j;

	if (tokens_list[0] && tokens_list[0][0] == '\0' && !ft_strchr("\'\"",
			main->input[skip_space(main->input)]))
	{
		free(tokens_list[0]);
		original_arg_count = main->cmd->arg_count;
		j = 0;
		while (j < original_arg_count - 1)
		{
			tokens_list[j] = tokens_list[j + 1];
			j++;
		}
		tokens_list[original_arg_count - 1] = NULL;
		main->cmd->arg_count = original_arg_count - 1;
		*token_count = main->cmd->arg_count;
	}
}
