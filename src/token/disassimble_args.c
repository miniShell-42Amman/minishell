/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disassimble_args.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 07:07:49 by lalhindi          #+#    #+#             */
/*   Updated: 2025/03/07 22:49:13 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_op(char *elem, char **new, int *i, char **split)
{
	char	*temp;
	char	*temp_two;

	temp = ft_strjoin("\"", elem);
	if (!temp)
		return (ft_free_split(split), 0);
	temp_two = ft_strjoin(temp, "\"");
	free(temp);
	if (!temp_two)
		return (ft_free_split(split), 0);
	new[(*i)++] = temp_two;
	return (1);
}

static int	handle_non_op(char *elem, char **new, int *i, char **split)
{
	new[*i] = ft_strdup(elem);
	if (!new[*i])
		return (ft_free_split(split), 0);
	(*i)++;
	return (1);
}

static int	populate_split(t_parse_cmd *p, char **new, int idx[3])
{
	char	**split;

	split = smart_split(p->cmd.args[idx[0]]);
	if (!split)
		return (0);
	idx[2] = -1;
	while (split[++idx[2]])
	{
		if (ft_is_operator(split[idx[2]]))
		{
			if (!handle_op(split[idx[2]], new, &idx[1], split))
				return (0);
		}
		else
		{
			if (!handle_non_op(split[idx[2]], new, &idx[1], split))
				return (0);
		}
	}
	ft_free_split(split);
	free(p->cmd.args[idx[0]]);
	p->cmd.args[idx[0]] = NULL;
	return (1);
}

static int	populate_new_args(t_parse_cmd *p, char **new, size_t size)
{
	int	idx[3];

	idx[0] = -1;
	idx[1] = 0;
	while (++idx[0] < p->cmd.arg_count && (size_t)idx[1] < size)
	{
		if (p->must_splitter[idx[0]] == 1)
		{
			if (!populate_split(p, new, idx))
				return (0);
		}
		else
		{
			new[idx[1]++] = p->cmd.args[idx[0]];
			p->cmd.args[idx[0]] = NULL;
		}
	}
	return (1);
}

int	disassemble_args(t_parse_cmd *p)
{
	char	**new_args;
	size_t	new_count;

	new_count = count_new_args(p);
	new_args = ft_calloc(new_count + 1, sizeof(char *));
	if (!new_args)
		return (EXIT_FAILURE);
	if (!populate_new_args(p, new_args, new_count))
	{
		ft_free_split(new_args);
		return (EXIT_FAILURE);
	}
	ft_free_split(p->cmd.args);
	p->cmd.args = new_args;
	p->cmd.arg_count = new_count;
	return (EXIT_SUCCESS);
}
