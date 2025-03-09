/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_fncs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 03:48:19 by lalhindi          #+#    #+#             */
/*   Updated: 2025/03/09 04:15:15 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void	count_dollars(t_parse_cmd *p, int *j)
{
	int	i;

	i = 0;
	*j = 0;
	while (p->splitter_clean_input[p->index_splitter][i])
	{
		if (p->splitter_clean_input[p->index_splitter][i] == '$')
			(*j)++;
		i++;
	}
}
void	process_dollars(t_parse_cmd *p, int *j, size_t *help)
{
	int	i;

	i = 0;
	*j = 0;
	*help = 0;
	while (p->splitter_clean_input[p->index_splitter][i])
	{
		if (p->splitter_clean_input[p->index_splitter][i] == '$')
		{
			if (p->splitter_clean_input[p->index_splitter][i + 1] == '?')
			{
				p->arr_has_dollar[(*j)++] = 1;
				i += 2;
				continue ;
			}
			*help = 0;
			i++;
			while (p->splitter_clean_input[p->index_splitter][i + *help]
				&& (ft_isalnum(p->splitter_clean_input[p->index_splitter][i + *help])
					|| p->splitter_clean_input[p->index_splitter][i + *help] == '_'))
				(*help)++;
			p->arr_has_dollar[(*j)++] = *help;
		}
		else
			i++;
	}
}
void		calculate_dollar_array(t_parse_cmd *p)
{
	int		j;
	size_t	help;

	count_dollars(p, &j);
	p->arr_has_dollar = malloc(sizeof(size_t) * (j + 1));
	if (!p->arr_has_dollar)
		return ;
    j = 0;    
	process_dollars(p, &j, &help);
	p->arr_has_dollar[j] = (size_t)-1;
}


void	cleanup_dollar_array(t_parse_cmd *p)
{
	if (p->arr_has_dollar)
	{
		free(p->arr_has_dollar);
		p->arr_has_dollar = NULL;
	}
}


