/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 21:45:29 by oissa             #+#    #+#             */
/*   Updated: 2025/02/20 22:43:06 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    cd(char **args, int arg_count, t_env *env_list)
{
    char    *path;

    if (arg_count < 2)
    {
        path = find_env_value(env_list, "HOME");
        if (!path)
        {
            ft_printf("cd: HOME not set\n");
            return ;
        }
    }
    else
        path = args[1];

    if (chdir(path) != 0)
        perror("cd");;
}
