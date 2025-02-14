/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 23:31:52 by lalhindi          #+#    #+#             */
/*   Updated: 2025/02/11 21:09:48 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_operator_length(char *input, int i)
{
	if (input[i] == '|')
		return (1);
	if (input[i] == '<' || input[i] == '>')
	{
		if (input[i + 1] == input[i])
			return (2);
		return (1);
	}
	return (0);
}

static int	is_inside_quotes(int in_single, int in_double)
{
	return (in_single || in_double);
}

static int	is_adjacent_to_quotes(char *input, int i, int op_len)
{
	if (i > 0 && (input[i - 1] == '\'' || input[i - 1] == '"'))
		return (1);
	if (input[i + op_len] == '\'' || input[i + op_len] == '"')
		return (1);
	return (0);
}

static void	process_token(char *input, t_counter *ct)
{
	int	op_len;

	while (input[ct->i] && (input[ct->i] != ' ' && input[ct->i] != '\t'))
	{
		if (input[ct->i] == '\'' && !ct->in_double)
			ct->in_single = !ct->in_single;
		else if (input[ct->i] == '"' && !ct->in_single)
			ct->in_double = !ct->in_double;
		op_len = get_operator_length(input, ct->i);
		if (op_len > 0)
		{
			if (is_inside_quotes(ct->in_single, ct->in_double)
				|| is_adjacent_to_quotes(input, ct->i, op_len))
			{
				if (!ct->array)
					ct->array = ft_calloc(sizeof(int), (count_args(input) + 1));
				ct->array[ct->count++] = ct->count_word - 1;
				ct->i += op_len;
				continue ;
			}
			else
				break ;
		}
		ct->i++;
	}
}

int	*ft_count_token(char *input)
{
	t_counter	ct;
	int			op_len;

	if (!input || !*input || input[0] == '|')
		return (NULL);
	ft_bzero(&ct, sizeof(t_counter));
	while (input[ct.i])
	{
		skip_whitespace(input, &ct.i, ft_strlen(input));
		if (!input[ct.i])
			break ;
		op_len = get_operator_length(input, ct.i);
		if (op_len > 0 && !is_inside_quotes(ct.in_single, ct.in_double)
			&& !is_adjacent_to_quotes(input, ct.i, op_len))
		{
			ct.count_word++;
			ct.i += op_len;
			continue ;
		}
		ct.count_word++;
		process_token(input, &ct);
	}
	if (ct.array)
		ct.array[ct.count] = -1;
	return (ct.array);
}
