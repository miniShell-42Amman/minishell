/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 22:21:03 by lalhindi          #+#    #+#             */
/*   Updated: 2025/03/07 22:30:20 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_syntax_error(char *token, int *status)
{
	ft_printf("syntax error near unexpected token `%s'\n", token);
	*status = 2;
	return (EXIT_FAILURE);
}

void	ft_perror_free_exit(char *msg, t_execute *execute, t_main *main,
		t_redirections *redirections)
{
	perror(msg);
	free_execute(execute, 0);
	free_resources(main, 1);
	free_redirections(redirections);
	exit(EXIT_FAILURE);
}
