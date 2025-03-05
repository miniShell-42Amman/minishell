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

int skip_space(char *str)
{
	int i;

	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	return (i);
}
// void skip_first_cmd(t_main *main)
// {

// }
// void ft_free_
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
	main->tokens_list = store_token(main->cmd->args, main->cmd->arg_count,
									array);

	// for(int i = 0; i < main->cmd->arg_count; i++)
	// {
	// 	if (main->tokens_list[i].value)
	// 		ft_printf("token[%d]: %s type %d\n", i, main->tokens_list[i].value, main->tokens_list[i].type);
	// }
	if (main->cmd->args)
	{
		ft_free_split(main->cmd->args);
		main->cmd->args = NULL;
	}
	if (main->cmd->cmd)
	{
		free(main->cmd->cmd);
		main->cmd->cmd = NULL;
	}
	int i = skip_space(main->input);
	if (main->tokens_list[0].value && !*main->tokens_list[0].value && !ft_strchr("\'\"", main->input[i]))
	{
		free(main->tokens_list[0].value);
		int j = -1;
		while (++j < main->cmd->arg_count - 1)
			main->tokens_list[j] = main->tokens_list[j + 1];
		main->cmd->arg_count--;
		main->exit_status = 0;
	}
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

void increment_shell_level(t_env **env_list)
{
	char *shell_level_str;
	int shell_level_int;
	char *shell_level_str_new;

	shell_level_str_new = ft_strdup("SHLVL");
	shell_level_str = get_env_var(*env_list, "SHLVL");
	if (shell_level_str)
	{
		shell_level_int = ft_atoi(shell_level_str);
		shell_level_int++;
	}
	else
	{
		shell_level_int = 1;
	}
	update_env(env_list, shell_level_str_new, ft_itoa(shell_level_int));
}
char *search_command(char *search)
{
	char *path = getenv("PATH");
	char **paths = ft_split(path, ':');
	char *command;
	char *tmp;
	int i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		command = ft_strjoin(tmp, search);
		if (access(command, F_OK) == 0)
		{
			ft_free_split(paths);
			free(tmp);
			return command;
		}
		free(tmp);
		free(command);
		i++;
	}
	ft_free_split(paths);
	return NULL;
}
int handle_many_args(int ac, char **av, t_main *main)
{
	if (ac > 1)
	{
		main->path = search_command(av[1]);
		if (main->path == NULL)
			ft_printf("%s: %s: cannot execute binary file\n", av[1], av[1]);
		else
			ft_printf("%s: %s\n", av[1], av[1]);
		free(main->path);
		main->exit_status = 126;
		return (EXIT_FAILURE);
	}
	main->program_name = av[0];
	return (EXIT_SUCCESS);
}
int main(int ac, char **av, char **env)
{
	t_main main;
	ft_bzero(&main, sizeof(t_main));
	if (handle_many_args(ac, av, &main))
		return (EXIT_FAILURE);
	main.exit_status = 0;
	main.env_list = clone_env(env);
	increment_shell_level(&main.env_list);
	if (!main.env_list)
		return (EXIT_FAILURE);
	setup_signals();
	while (1)
	{
		g_signal = 0;
		free_resources(&main, 0);
		if (g_signal)
		{
			main.exit_status = g_signal;
			g_signal = 0;
		}
		main.input = readline(PROMPT);
		if (!main.input || g_signal == 2000)
		{
			ft_printf("exit\n");
			free(main.input);
			main.input = NULL;
			break;
		}
		// if (ft_strcmp(main.input, "exit") == 0)
		// {
		// 	free_resources(&main, 1);
		// 	break;
		// }
		if (ft_strlen(main.input) > 0)
			add_history(main.input);
		main.skip_spaces = skip_space(main.input);
		if (main.input[main.skip_spaces] != '\0' && *main.input && !start_tokenization(&main))
		{
			free(main.input);
			main.input = NULL;
			start_execution(&main);
		}
		free_resources(&main, 0);
	}
	free_resources(&main, 1);
	rl_clear_history();
	return (0);
}
