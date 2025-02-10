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

void	start_tokenization(char *input)
{
	int	*array;
	int	i;

	i = 0;
	array = ft_count_token(input);
	if (array == NULL)
	{
		fprintf(stderr, "Error: ft_count_token returned NULL\n");
		return ;
	}
	while (array[i] != -1)
	{
		printf("%d\n", array[i]);
		i++;
	}
	free(array);
}

char	*find_env_value(t_env *env_list, const char *var_name)
{
	t_env	*current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
			return (current->value);
		current = current->next;
	}
	return ("");
}

int	main(int ac, char **av, char **env)
{
	t_main	main;

	(void)ac;
	(void)av;
	// int not_valid;
	main.env_list = clone_env(env);
	if (!main.env_list)
	{
		perror("malloc");
		return (EXIT_FAILURE);
	}
	while (1)
	{
		main.input = readline("minishell$ ");
		if (ft_strcmp(main.input, "exit") == 0)
		{
			// free_reosurces(main.input, main.env_list);
			break ;
		}
		add_history(main.input);
		start_tokenization(main.input);
	}
	free_env_list(main.env_list);
	free(main.input);
	return (0);
}
