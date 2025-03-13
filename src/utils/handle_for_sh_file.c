/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_for_sh_file.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:35:31 by oissa             #+#    #+#             */
/*   Updated: 2025/03/13 16:36:17 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	loop_get_next_line(t_main *main)
{
	while (main->input)
	{
		main->skip_spaces = skip_space(main->input);
		if (main->input[main->skip_spaces] == '\0')
		{
			free_resources(main, 0);
			main->input = get_next_line(main->fd);
			continue ;
		}
		if (main->input[main->skip_spaces] != '\0' && *main->input
			&& !start_tokenization(main))
		{
			free(main->input);
			main->input = NULL;
			start_execution(main);
		}
		free_resources(main, 0);
		main->input = get_next_line(main->fd);
	}
}

int	check_shell_args(char **av, char **env, t_main *main)
{
	if (ft_strnstr(av[1], ".sh", ft_strlen(av[1])) != NULL)
	{
		main->fd = open(av[1], O_RDONLY);
		main->env_list = clone_env(env);
		if (main->fd == -1)
		{
			ft_printf("%s: %s: No such file or directory\n", av[0], av[1]);
			main->exit_status = 127;
			close(main->fd);
			free_resources(main, 1);
			return (EXIT_FAILURE);
		}
		main->input = get_next_line(main->fd);
		loop_get_next_line(main);
		free_resources(main, 1);
		return (2);
	}
	return (EXIT_SUCCESS);
}
