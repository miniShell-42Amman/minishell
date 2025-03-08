/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smart_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:03:47 by lalhindi          #+#    #+#             */
/*   Updated: 2025/03/08 23:06:44 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_array_size(char **array)
{
	size_t	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

int	count_quoted_words(const char *str)
{
	t_split	s;

	ft_bzero(&s, sizeof(s));
	while (str[s.i])
	{
		update_quote_state(str[s.i], &s.sq, &s.dq);
		if ((!s.sq && !s.dq) && (str[s.i] == ' ' || str[s.i] == '|'
				|| str[s.i] == '<' || str[s.i] == '>'))
		{
			if (s.i > s.start)
				s.wc++;
			if (str[s.i] == '|' || str[s.i] == '<' || str[s.i] == '>')
				s.wc++;
			s.start = s.i + 1;
		}
		s.i++;
	}
	if (s.i > s.start)
		s.wc++;
	return (s.wc);
}

void	loop_smart_split(const char *str, t_split *s, char **res,
		const char *original)
{
	while (str[s->i])
	{
		update_quote_state(str[s->i], &s->sq, &s->dq);
		if ((!s->sq && !s->dq) && (str[s->i] == ' ' || str[s->i] == '|'
				|| str[s->i] == '<' || str[s->i] == '>'))
		{
			if (s->i > s->start)
				res[s->wc++] = ft_substr(original, s->start, s->i - s->start);
			if (str[s->i] == '|' || str[s->i] == '<' || str[s->i] == '>')
			{
				if (str[s->i + 1] == str[s->i])
					res[s->wc++] = ft_substr(original, s->i, 2);
				else
					res[s->wc++] = ft_substr(original, s->i, 1);
			}
			if (str[s->i + 1] == str[s->i])
				s->start = s->i + 2;
			else
				s->start = s->i + 1;
			if (str[s->i + 1] == str[s->i])
				s->i++;
		}
		s->i++;
	}
}

char	**smart_split(const char *str)
{
	char		**res;
	t_split		s;
	const char	*original;
	int			word_count;

	original = str;
	ft_bzero(&s, sizeof(t_split));
	word_count = count_quoted_words(str);
	res = (char **)ft_calloc(word_count + 1, sizeof(char *));
	if (!res)
		return (NULL);
	loop_smart_split(str, &s, res, original);
	if (s.i > s.start)
		res[s.wc++] = ft_substr(original, s.start, s.i - s.start);
	res[s.wc] = NULL;
	return (res);
}
