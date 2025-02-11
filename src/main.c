/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 22:31:26 by oissa             #+#    #+#             */
/*   Updated: 2025/02/10 23:36:40 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	start_tokenization(t_main *main)
{
	int	*array;
	int	i;

	i = 0;
	array = ft_count_token(main->input);
    main->cmd = parse_cmd(main->input, main->env_list);
    if (!main->cmd.args)
    {
        ft_printf("Error: parse_cmd returned NULL\n");
        free(array);
        return;
    }
    // while (main->cmd.args[i])
    // {
    //     printf("args[%d]: %s\n", i, main->cmd.args[i]);
    //     i++;
    // }
	main->tokens_list = store_token(main->cmd.args, main->cmd.arg_count, array);
	if (!main->tokens_list)
	{
		ft_printf("Error: store_token returned NULL\n");
		free(array);
		return;
	}
	i = 0;
	while (main->tokens_list[i].value)
	{
		if (main->tokens_list[i].value != NULL)
			printf("Token[%d]: %s => %d\n", i, main->tokens_list[i].value, main->tokens_list[i].type);
		else
			printf("Token[%d]: (null) => %d\n", i, main->tokens_list[i].type);
		free(main->tokens_list[i].value);
		i++;
	}
	free(main->tokens_list);
	free_command(&main->cmd);
    free(array);
}

int	main(int ac, char **av, char **env)
{
	t_main	main;

	(void)ac;
	(void)av;
	// int not_valid;
	ft_bzero(&main, sizeof(t_main));
	main.env_list = clone_env(env);
	if (!main.env_list)
	{
		perror("malloc");
		return (EXIT_FAILURE);
	}
	while (1)
	{
		main.input = readline("\001\033[35m\002⚠️  Error404 ⚠️  >\001\033[34m\002 ");
		if (ft_strcmp(main.input, "exit") == 0)
		{
			// free_reosurces(main.input, main.env_list);
			break ;
		}
		add_history(main.input);
		start_tokenization(&main);
	}
	free_env_list(main.env_list);
	free(main.input);
	return (0);
}
