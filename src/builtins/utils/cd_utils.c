/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 00:06:21 by oissa             #+#    #+#             */
/*   Updated: 2025/03/08 00:08:01 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_cd_error(const char *cmd, const char *arg)
{
	ft_dprintf(2, "⚠️  Error404 ⚠️ : %s: %s\n", cmd, arg);
	return (1);
}
