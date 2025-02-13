/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 23:19:03 by oissa             #+#    #+#             */
/*   Updated: 2025/02/12 00:38:56 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_operator(const char *str)
{
	if (*str == '|')
		return (1);
	if (*str == '>' || *str == '<')
	{
		if (*(str + 1) == *str)
			return (2);
		return (1);
	}
	return (0);
}

static void	skip_whitespace(const char *input, int *i, int len)
{
	while (*i < len && (input[*i] == ' ' || input[*i] == '\t'))
		(*i)++;
}

static void	handle_quotes(char *input, t_counter *c)
{
	while (c->i < c->len)
	{
		if (input[c->i] == '\'' && !(c->in_double))
			c->in_single = !(c->in_single);
		else if (input[c->i] == '"' && !(c->in_single))
			c->in_double = !(c->in_double);
		if (!(c->in_single) && !(c->in_double))
		{
			if (input[c->i] == ' ' || input[c->i] == '\t' || input[c->i] == '|'
				|| ft_is_operator(&input[c->i]))
				break ;
		}
		(c->i)++;
	}
}

int	count_args(char *input)
{
	t_counter	c;

	ft_bzero(&c, sizeof(t_counter));
	c.len = ft_strlen(input);
	while (c.i < c.len)
	{
		skip_whitespace(input, &c.i, c.len);
		if (c.i >= c.len)
			break ;
		if (input[c.i] == '|')
		{
			c.count++;
			c.i++;
			continue ;
		}
		c.count++;
		c.in_single = 0;
		c.in_double = 0;
		handle_quotes(input, &c);
		// c.i++;
	}
	return (c.count);
}
