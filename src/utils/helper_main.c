/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 21:56:49 by lalhindi          #+#    #+#             */
/*   Updated: 2025/03/16 04:05:33 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_many_args(int ac, char **av, char **env, t_main *main)
{
	if (ac > 1)
	{
		if (check_shell_args(av, env, main))
			return (EXIT_FAILURE);
		main->path = search_command(av[1]);
		if (main->path)
			ft_printf("%s: %s: cannot execute binary file\n", main->path,
				main->path);
		else
			ft_printf("%s: %s: No such file or directory\n", av[0], av[1]);
		free(main->path);
		main->exit_status = 126;
		return (EXIT_FAILURE);
	}
	main->program_name = av[0];
	return (EXIT_SUCCESS);
}

int	help_loop(t_main *main)
{
	main->input = readline(PROMPT);
	if (!main->input)
	{
		ft_printf("exit\n");
		free(main->input);
		main->input = NULL;
		return (EXIT_FAILURE);
	}
	if (g_signal == 130)
	{
		main->exit_status = g_signal;
		g_signal = 0;
	}
	return (EXIT_SUCCESS);
}

void	loop_main(t_main *main)
{
	while (1)
	{
		free_resources(main, 0);
		if (help_loop(main))
			break ;
		if (ft_strlen(main->input) == 0)
		{
			free(main->input);
			main->input = NULL;
			continue ;
		}
		add_history(main->input);
		main->skip_spaces = skip_space(main->input);
		if (main->input[main->skip_spaces] != '\0' && *main->input
			&& !start_tokenization(main))
		{
			free(main->input);
			main->input = NULL;
			start_execution(main);
		}
		free_resources(main, 0);
	}
}

int	skip_space(char *str)
{
	int	i;

	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	return (i);
}
