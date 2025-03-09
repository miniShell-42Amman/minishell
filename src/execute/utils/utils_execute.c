/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_execute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 04:01:01 by oissa             #+#    #+#             */
/*   Updated: 2025/03/09 04:09:38 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_error(char *message, t_execute *execute)
{
	ft_dprintf(STDERR_FILENO, "%s\n", message);
	free_execute(execute, 0);
	exit(EXIT_FAILURE);
}

size_t	calculate_number_operations(t_token *tokens, size_t token_count)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (i < token_count)
	{
		if (tokens[i].type == TOKEN_PIPE)
			count++;
		i++;
	}
	return (count);
}

int	is_directory(const char *path)
{
	struct stat	statbuf;
	int			result;

	if (stat(path, &statbuf) != 0)
		return (0);
	result = S_ISDIR(statbuf.st_mode);
	return (result);
}

void	check_command(int flag, int *i, int *max)
{
	if (flag == 1)
	{
		*i = 3;
		*max = 7;
	}
	else if (flag == 2)
	{
		*i = 0;
		*max = 7;
	}
}

int	is_commands(t_execute *execute, int flag)
{
	char	*customs[7];
	int		i;
	int		max;

	customs[0] = "echo";
	customs[1] = "env";
	customs[2] = "pwd";
	customs[3] = "exit";
	customs[4] = "export";
	customs[5] = "unset";
	customs[6] = "cd";
	i = 0;
	max = 4;
	check_command(flag, &i, &max);
	while (i < max)
	{
		if (execute->commands[execute->i][0]
			&& ft_strcmp(execute->commands[execute->i][0], customs[i]) == 0)
			return (EXIT_SUCCESS);
		i++;
	}
	return (EXIT_FAILURE);
}
