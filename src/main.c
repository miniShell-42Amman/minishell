/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 22:31:26 by oissa             #+#    #+#             */
/*   Updated: 2025/02/14 19:33:50 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_resources(t_main *main, int flag)
{
	if (main->input)
	{
		free(main->input);
		main->input = NULL;
	}
	if (main->env_list && flag)
	{
		free_env_list(main->env_list);
		main->env_list = NULL;
	}
	if (main->tokens_list && main->cmd)
	{
		free_tokens(main->tokens_list, main->cmd->arg_count);
		main->tokens_list = NULL;
	}
	if (main->cmd)
	{
		free_command(main->cmd);
		main->cmd = NULL;
	}
}
int	is_duplicate_operator_series(t_token *token, int token_count)
{
	int	i;

	i = 0;
	while (i < token_count - 1)
	{
		if (token[i].type >= 2 && token[i + 1].type >= 2)
		{
			ft_printf("Error404: syntax error near unexpected token \'%s\'\n",
				token[i + 1].value);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

void	start_tokenization(t_main *main)
{
	int	*array;

	array = ft_count_token(main->input);
	if (array)
	{
		for (int i = 0; i < array[0]; i++)
		{
			if (array[i])
				ft_printf("array[%d]: %d\n", i, array[i]);
		}	
	}
	main->cmd = parse_cmd(main->input, main->env_list);
	if (!main->cmd)
	{
		ft_printf("Error: parse_cmd returned NULL\n");
		free_resources(main, 0);
		if (array)
			free(array);
		return ;
	}
	main->tokens_list = store_token(main->cmd->args, main->cmd->arg_count,
			array);
	if (!main->tokens_list || is_duplicate_operator_series(main->tokens_list,
			main->cmd->arg_count))
	{
		free_resources(main, 0);
		free(array);
		return ;
	}
	for (int k = 0; k < (*main->cmd).arg_count; k++)
	{
		if (main->tokens_list[k].value)
			ft_printf("Token[%d]: %s => %d\n", k, main->tokens_list[k].value,
				main->tokens_list[k].type);
	}
	free(array);
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
		main.input = readline("\001\033[35m\002⚠️  Error404 ⚠️  >\001\033[34m\002 ");
		if (ft_strcmp(main.input, "exit") == 0)
			break ;
		add_history(main.input);
		start_tokenization(&main);
		free(main.input);
		main.input = NULL;
	}
	free_resources(&main, 1);
	rl_clear_history();
	return (0);
}
