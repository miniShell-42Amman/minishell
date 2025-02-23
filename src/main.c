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

volatile sig_atomic_t g_signal = 0;

int start_tokenization(t_main *main)
{
	int *array;

	array = ft_count_token(main->input);
	main->cmd = parse_cmd(main->input, main->env_list, &main->exit_status);
	if (!main->cmd)
	{
		free_resources(main, 0);
		if (array)
			free(array);
		return (EXIT_FAILURE);
	}
	main->tokens_list = store_token(main->cmd->args, main->cmd->arg_count,
									array);
	if (!main->tokens_list || is_duplicate_operator_series(main->tokens_list,
														   main->cmd->arg_count, &main->exit_status))
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

void handle_sigint(int signum)
{
    (void)signum;
	if(!g_signal || g_signal == 130)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);    
		rl_on_new_line();  
		// rl_redisplay();
	}
    g_signal = 130;         
}
void handle_sigquit(int signum)
{
	(void)signum;
	if(!g_signal || g_signal == 131)
	{
		write(1, "Quit: 3\n", 8);
	}
	g_signal = 131;
}
void setup_signals(void)
{
    struct sigaction sa_int, sa_quit;

    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_int, NULL);

    sa_quit.sa_handler = handle_sigquit;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);
}

int main(int ac, char **av, char **env)
{
	t_main main;

	(void)ac;
	(void)av;
	ft_bzero(&main, sizeof(t_main));
	main.exit_status = 0;
	main.env_list = clone_env(env);
	if (!main.env_list)
	{
		perror("malloc");
		return (EXIT_FAILURE);
	}
	setup_signals();
	while (1)
	{
		free_resources(&main, 0);
		main.input = readline(PROMPT);
		if (g_signal)
		{ 
			main.exit_status = g_signal;
			g_signal = 0;
		}
		if (!main.input)
		{
			ft_printf("exit\n");
			break;
		}
		if (ft_strcmp(main.input, "exit") == 0)
			break;
		add_history(main.input);
		main.str = ft_strtrim(main.input, " ");
		if (*main.str && *main.input && !start_tokenization(&main))
			start_execution(main.tokens_list, main.cmd->arg_count, main.env_list, &main.exit_status);
		free(main.input);
		free(main.str);
		main.str = NULL;
		main.input = NULL;
	}
	free_resources(&main, 1);
	rl_clear_history();
	return (0);
}
