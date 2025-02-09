/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 22:31:26 by oissa             #+#    #+#             */
/*   Updated: 2025/02/10 01:26:42 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void start_tokenization(char *input, t_env *env_list)
{
	int *array;
	
	array = ft_count_token(input);
	if (!array)
	{
		ft_dprintf(2, "syntax error near unexpected token '|'\n");
		return;
	}
}

char *find_env_value(t_env *env_list, const char *var_name)
{
    t_env *current = env_list;
    while (current)
    {
        if (ft_strcmp(current->key, var_name) == 0)
            return current->value;
        current = current->next;
    }
    return "";
}

int	main(int ac, char **av, char **env)
{
	t_main	main;
	(void)ac;
	(void)av;
	main.env_list = clone_env(env);
	if (!main.env_list)
	{
		perror("malloc");
		return (EXIT_FAILURE);
	}
    
    while (1)
    {
        main.input = readline("minishell$ ");
        add_history(main.input);
		start_tokenization(main.input, main.env_list);
    }
	free_env_list(main.env_list);
    free(main.input);
	return (0);
}
