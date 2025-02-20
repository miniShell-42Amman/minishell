/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 21:57:26 by oissa             #+#    #+#             */
/*   Updated: 2025/02/20 22:43:28 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    pwd(void)
{
    char    *path;

    path = getcwd(NULL, 0);
    if (!path)
        return ;
    ft_printf("%s\n", path);
    free(path);
}
