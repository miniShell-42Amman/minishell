/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:11:00 by lalhindi          #+#    #+#             */
/*   Updated: 2025/03/13 17:08:43 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_operator(char *token, int pos, char **tokens)
{
	(void)pos;
	(void)tokens;
	if (ft_strchr(token, '\'') || ft_strchr(token, '"'))
		return (0);
	if (ft_strcmp(token, "|") == 0)
	{
		return (1);
	}
	if (ft_strcmp(token, ">") == 0 || ft_strcmp(token, "<") == 0
		|| ft_strcmp(token, ">>") == 0 || ft_strcmp(token, "<<") == 0)
	{
		return (1);
	}
	return (1);
}

static int	is_have_operation(char *token)
{
	if (ft_strchr(token, '|') == 0 || ft_strchr(token, '>') == 0
		|| ft_strchr(token, '<') == 0 || ft_strnstr(token, ">>", 2)
		|| ft_strnstr(token, "<<", 2))
		return (1);
	return (0);
}

static void	validate_tokens(char **tokens, t_counter *ct)
{
	int	i;

	i = -1;
	while (tokens[++i])
	{
		if (is_have_operation(tokens[i]) && !is_valid_operator(tokens[i], i,
				tokens))
		{
			if (!ct->array)
				ct->array = ft_calloc(ct->count_word + 1, sizeof(int));
			ct->array[ct->count++] = i;
		}
	}
	if (ct->array)
		ct->array[ct->count] = -1;
}

int	*ft_count_token(char *input)
{
	t_counter	ct;
	char		**tokens;

	ft_bzero(&ct, sizeof(t_counter));
	tokens = smart_split(input);
	if (!tokens)
		return (NULL);
	while (tokens[ct.count_word])
		ct.count_word++;
	validate_tokens(tokens, &ct);
	ft_free_split(tokens);
	return (ct.array);
}
