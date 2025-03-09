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
	int original_arg_count;
	t_token *new_tokens;

	main->tokens_list = store_token(main->cmd->args, main->cmd->arg_count, array);
	if (!main->tokens_list)
		return (EXIT_FAILURE);
	if (main->tokens_list[0].value && !*main->tokens_list[0].value &&
		!ft_strchr("\'\"", main->input[skip_space(main->input)]))
	{
		free(main->tokens_list[0].value);
		main->tokens_list[0].value = NULL;

		original_arg_count = main->cmd->arg_count;
		new_tokens = malloc((original_arg_count - 1) * sizeof(t_token));
		if (!new_tokens)
		{
			main->exit_status = 1;
			free_tokens(main->tokens_list, original_arg_count);
			return (EXIT_FAILURE);
		}
		for (int j = 0; j < original_arg_count - 1; j++)
		{
			printf("main->tokens_list[%d].value: %s\n", j, main->tokens_list[j].value);
			new_tokens[j] = main->tokens_list[j + 1];
		}
		main->tokens_list = new_tokens;
		main->cmd->arg_count = original_arg_count - 1;
		if (main->cmd->arg_count > 0)
			main->tokens_list[0].type = TOKEN_COMMAND;
		main->exit_status = 0;
	}
	for (int i = 0; i < main->cmd->arg_count; i++)
	{
		printf("main->tokens_list[%d].value: %s\n", i, main->tokens_list[i].value);
	}
	return (EXIT_SUCCESS);
}

// int get_value_store_token(t_main *main, int *array)
// {
// 	// int	j;
// 	// t_token	*tmp;
// 	main->tokens_list = store_token(main->cmd->args, main->cmd->arg_count,
// 									array);
// 	if (main->tokens_list[0].value && !*main->tokens_list[0].value && !ft_strchr("\'\"", main->input[skip_space(main->input)]))
// 	{
// 		free(main->tokens_list[0].value);
// 		int original_arg_count = main->cmd->arg_count;
// 		t_token *new_tokens = malloc((original_arg_count - 1) * sizeof(t_token));
// 		if (!new_tokens)
// 		{
// 			main->exit_status = 1;
// 			return (EXIT_FAILURE);
// 		}

// 		for (int j = 0; j < original_arg_count - 1; j++)
// 			new_tokens[j] = main->tokens_list[j + 1];

// 		free_tokens(main->tokens_list, original_arg_count);
// 		main->cmd->arg_count = original_arg_count - 1;
// 		t_token *tokens_list = malloc(main->cmd->arg_count * sizeof(t_token));
// 		if (!tokens_list)
// 		{
// 			main->exit_status = 1;
// 			return (EXIT_FAILURE);
// 		}
// 		for (int j = 0; j < main->cmd->arg_count; j++)
// 			tokens_list[j] = new_tokens[j];
// 		free_tokens(new_tokens, original_arg_count - 1);
// 		main->tokens_list[0].type = TOKEN_COMMAND;
// 		main->exit_status = 0;
// 	}
// 	return (EXIT_SUCCESS);
// }

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
	if (!main->cmd->arg_count || !main->tokens_list[0].value || is_duplicate_operator_series(main->tokens_list, main->cmd->arg_count, &main->exit_status))
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
