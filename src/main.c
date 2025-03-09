/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 22:31:26 by oissa             #+#    #+#             */
/*   Updated: 2025/03/09 20:50:50:44 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_signal = 0;

int get_value_store_token(t_main *main, int *array)
{
    main->tokens_list = store_token(main->cmd->args, main->cmd->arg_count, array, main);
    if (!main->tokens_list)
        return (EXIT_FAILURE);
    return (EXIT_SUCCESS);
}


int start_tokenization(t_main *main)
{
	int *array;

	array = ft_count_token(main->input);
	main->cmd = parse_cmd(main);
	if (!main->cmd)
	{
		free_resources(main, 0);
		if (array)
			free(array);
		return (EXIT_FAILURE);
	}
	get_value_store_token(main, array);
	if (!main->cmd->arg_count || !main->tokens_list[0].value
		|| is_duplicate_operator_series(main->tokens_list, main->cmd->arg_count, &main->exit_status))
	{
		free_resources(main, 0);
		free(array);
		return (EXIT_FAILURE);
	}
	if (array)
		free(array);
	main->exit_status = 0;
	return (EXIT_SUCCESS);
}

int main(int ac, char **av, char **env)
{
	t_main main;

	ft_bzero(&main, sizeof(t_main));
	if (handle_many_args(ac, av, &main))
		return (EXIT_FAILURE);
	main.env_list = clone_env(env);
	increment_shell_level(&main.env_list);
	if (!main.env_list)
		return (EXIT_FAILURE);
	setup_signals();
	loop_main(&main);
	free_resources(&main, 1);
	rl_clear_history();
	return (0);
}
