/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_disassimble.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 22:49:10 by lalhindi          #+#    #+#             */
/*   Updated: 2025/03/07 22:49:51 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	count_new_args(t_parse_cmd *p)
{
	size_t	count;
	int		i;
	char	**split;

	count = p->cmd.arg_count;
	i = 0;
	while (i < p->cmd.arg_count)
	{
		if (p->must_splitter[i] == 1)
		{
			split = ft_split(p->cmd.args[i], ' ');
			count += ft_array_size(split) - 1;
			ft_free_split(split);
		}
		i++;
	}
	return (count);
}
