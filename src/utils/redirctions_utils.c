/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirctions_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 00:44:40 by oissa             #+#    #+#             */
/*   Updated: 2025/03/14 00:54:21 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection(char *str)
{
	if (ft_strcmp(str, ">") == 0 || ft_strcmp(str, ">>") == 0 || ft_strcmp(str,
			"<") == 0)
		return (1);
	return (0);
}

void	clean_arguments(t_execute *execute, t_redirections *r)
{
	char	**new_argv;
	int		i;
	int		k;

	new_argv = ft_calloc(ft_arr_len(r->argv) + 1, sizeof(char *));
	if (!new_argv)
		return ;
	i = -1;
	k = 0;
	while (r->argv[++i])
	{
		if (is_redirection(r->argv[i]) && r->argv[i + 1])
		{
			i++;
			continue ;
		}
		new_argv[k++] = ft_strdup(r->argv[i]);
	}
	execute->commands[execute->i] = ft_free_split(
			execute->commands[execute->i]);
	execute->commands[execute->i] = new_argv;
}

size_t	ft_determine_number_of_commands(t_execute *execute)
{
	size_t	num_command;
	size_t	i;
	size_t	n_commands;

	i = 0;
	n_commands = 0;
	num_command = 0;
	while (i < execute->i)
	{
		while (execute->commands[i][n_commands])
		{
			n_commands++;
		}
		if (execute->i > 0)
			num_command++;
		num_command += n_commands;
		n_commands = 0;
		i++;
	}
	return (num_command);
}

int	check_target_redirecion(t_redirections *redirections, t_execute *execute,
		int i)
{
	if (!redirections->argv[i + 1])
	{
		ft_dprintf(2, "Syntax error near token '%s'\n",
			redirections->argv[i]);
		*execute->exit_status = 258;
		return (EXIT_BREAK);
	}
	return (EXIT_FAILURE);
}
