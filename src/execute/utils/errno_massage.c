/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errno_massage.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:45:01 by oissa             #+#    #+#             */
/*   Updated: 2025/03/13 17:10:41 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_errno_massage(t_execute *execute)
{
	ft_dprintf(2, "Error404: %s: %s\n", execute->commands[execute->i][0],
		strerror(errno));
}

void	errno_massage(t_execute *execute)
{
	if (errno == ENOENT)
	{
		ft_dprintf(2, "Error404: command not found");
		*execute->exit_status = 127;
	}
	else if (errno == EACCES)
	{
		print_errno_massage(execute);
		*execute->exit_status = 126;
	}
	else if (errno == ENOMEM)
	{
		print_errno_massage(execute);
		*execute->exit_status = 1;
	}
	else if (errno == EFAULT)
	{
		print_errno_massage(execute);
		*execute->exit_status = 1;
	}
	else
	{
		print_errno_massage(execute);
		*execute->exit_status = 126;
	}
}
