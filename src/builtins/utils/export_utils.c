/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 03:04:40 by lalhindi          #+#    #+#             */
/*   Updated: 2025/03/08 00:36:19 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quotes(char *str)
{
	char	*clean;
	int		len;

	clean = NULL;
	len = ft_strlen(str);
	if (len < 2)
		return (ft_strdup(str));
	if ((str[0] == '"' && str[len - 1] == '"') || (str[0] == '\'' && str[len
				- 1] == '\''))
		clean = ft_substr(str, 1, len - 2);
	if (clean)
		return (clean);
	return (ft_strdup(str));
}
