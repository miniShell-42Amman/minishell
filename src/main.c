/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 22:31:26 by oissa             #+#    #+#             */
/*   Updated: 2025/02/11 21:12:30 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void free_reosurces(t_main *main , int flag)
{
    if (main->input)
		free(main->input);
    if (main->env_list && flag) 
		free_env_list(main->env_list);
	if (main->tokens_list && main->cmd)
            free_tokens(main->tokens_list, main->cmd->arg_count);
	if (main->cmd) 
		free_command(main->cmd);
}

void	start_tokenization(t_main *main)
{
	int	*array;
	
	array = ft_count_token(main->input);
	main->cmd = parse_cmd(main->input, main->env_list);
    if (!(*main->cmd).args || !main->cmd)
    {
        ft_printf("Error: parse_cmd returned NULL\n");
        free(array);
        return;
    }
	main->tokens_list = store_token(main->cmd->args, main->cmd->arg_count, array);
	if (!main->tokens_list)
	{
		ft_printf("Error: store_token returned NULL\n");
		free_reosurces(main, 0);
		free_command(main->cmd);
		main->cmd = NULL;
		free(array);
		return;
	}
	// free_tokens(main->tokens_list, main->cmd->arg_count);
	// for(int k = 0; k < (*main->cmd).arg_count; k++)
	// {
	// 	if (main->tokens_list[k].value)
	// 	printf("Token[%d]: %s => %d\n", k, main->tokens_list[k].value, main->tokens_list[k].type);
	// }
	// free(main->tokens_list);
	// free_command(main->cmd);
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
		main.input = readline("\001\033[35m\002⚠️  Error404 ⚠️  >\001\033[34m\002 ");
		if (ft_strcmp(main.input, "exit") == 0)
			break ;
		add_history(main.input);
		start_tokenization(&main);
	}
	free_reosurces(&main,1);
	rl_clear_history();
	return (0);
}
