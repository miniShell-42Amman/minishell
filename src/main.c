/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 22:31:26 by oissa             #+#    #+#             */
/*   Updated: 2025/02/15 18:25:02 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_signal = 0;

void get_value_store_token(t_main *main, int *array)
{
	int j;

	main->tokens_list = store_token(main->cmd->args, main->cmd->arg_count, array);
	if (main->tokens_list[0].value && !*main->tokens_list[0].value
		&&!ft_strchr("\'\"", main->input[skip_space(main->input)]))
	{
		free(main->tokens_list[0].value);
		j = -1;
		while (++j < main->cmd->arg_count - 1)
			main->tokens_list[j] = main->tokens_list[j + 1];
		main->cmd->arg_count--;
		main->exit_status = 0;
	}
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
