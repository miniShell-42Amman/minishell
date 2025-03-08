/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:08:42 by oissa             #+#    #+#             */
/*   Updated: 2025/03/08 22:39:01 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_dolloar_quote(const char *token)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (!token || !*token)
		return (0);
	while (token[i] && token[i] != ' ')
	{
		if (token[i] == '$')
			count++;
		i++;
	}
	return (count);
}

int	ft_arr_len(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return (0);
	while (arr[i])
		i++;
	return (i);
}

void	update_quote_state(char c, bool *squote, bool *dquote)
{
	if (c == '\'' && !*dquote)
		*squote = !*squote;
	else if (c == '"' && !*squote)
		*dquote = !*dquote;
}
