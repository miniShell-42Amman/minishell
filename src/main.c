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

int	start_tokenization(t_main *main)
{
	int	*array;

	array = ft_count_token(main->input);
	main->cmd = parse_cmd(main->input, main->env_list);
	if (!main->cmd)
	{
		ft_printf("Error: parse_cmd returned NULL\n");
		free_resources(main, 0);
		if (array)
			free(array);
		return (EXIT_FAILURE);
	}
	main->tokens_list = store_token(main->cmd->args, main->cmd->arg_count,
			array);
	if (!main->tokens_list || is_duplicate_operator_series(main->tokens_list,
			main->cmd->arg_count))
	{
		free_resources(main, 0);
		free(array);
		return (EXIT_FAILURE);
	}
	if (array)
		free(array);
	return (EXIT_SUCCESS);
}

int	main(int ac, char **av, char **env)
{
	t_main	main;

	(void)ac;
	(void)av;
	ft_bzero(&main, sizeof(t_main));
	main.env_list = clone_env(env);
	if (!main.env_list)
	{
		perror("malloc");
		return (EXIT_FAILURE);
	}
	while (1)
	{
		free_resources(&main, 0);
		main.input = readline("\001\033[35m\002⚠️  Error404 ⚠️  >\001\033[0;32m\002 ");
		if (ft_strcmp(main.input, "exit") == 0)
			break ;
		add_history(main.input);
		if(!start_tokenization(&main))
			start_execution(main.tokens_list, main.cmd->arg_count, main.env_list);
		free(main.input);
		main.input = NULL;
	}
	free_resources(&main, 1);
	rl_clear_history();
	return (0);
}
