/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 21:57:26 by oissa             #+#    #+#             */
/*   Updated: 2025/02/21 16:34:36 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    pwd(void)
{
    char    path[PATH_MAX];
    if (!getcwd(path, PATH_MAX))
        return ;
    ft_printf("%s\n", path);
}
