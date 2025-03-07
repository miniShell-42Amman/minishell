/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 21:49:48 by oissa             #+#    #+#             */
/*   Updated: 2025/02/20 21:49:48 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>
#include <errno.h>

void exit_error(char *message, t_execute *execute)
{
    ft_dprintf(STDERR_FILENO, "%s\n", message);
    free_execute(execute, 0);
    exit(EXIT_FAILURE);
}

size_t calculate_number_operations(t_token *tokens, size_t token_count)
{
    size_t i;
    size_t count;

    i = 0;
    count = 0;
    while (i < token_count)
    {
        if (tokens[i].type == TOKEN_PIPE)
            count++;
        i++;
    }
    return (count);
}

int init_command(t_token *token, size_t token_count, t_execute *execute)
{
    execute->commands = ft_calloc(execute->num_cmds, sizeof(char **));
    execute->cmd_args = ft_calloc(execute->num_cmds, sizeof(size_t));
    if (!execute->commands || !execute->cmd_args)
        return (EXIT_FAILURE);
    execute->i = -1;
    while (++execute->i < token_count)
    {
        if (token[execute->i].type == TOKEN_PIPE)
            execute->cmd_index++;
        else
            execute->cmd_args[execute->cmd_index]++;
    }
    execute->i = -1;
    while (++execute->i < execute->num_cmds)
    {
        execute->commands[execute->i] = ft_calloc(execute->cmd_args[execute->i] + 1, sizeof(char *));
        if (!execute->commands[execute->i])
            return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

int fill_commands(t_token *token, size_t token_count, t_execute *execute)
{
    if (init_command(token, token_count, execute))
        return (EXIT_FAILURE);
    execute->i = -1;
    execute->cmd_index = 0;
    execute->arg_index = 0;
    while (++execute->i < token_count)
    {
        if (token[execute->i].type == TOKEN_PIPE)
        {
            execute->cmd_index++;
            execute->arg_index = 0;
        }
        else
        {
            execute->commands[execute->cmd_index][execute->arg_index] = ft_strdup(token[execute->i].value);
            if (!execute->commands[execute->cmd_index][execute->arg_index])
                return (EXIT_FAILURE);
            execute->arg_index++;
            free(token[execute->i].value);
            token[execute->i].value = NULL;
        }
    }
    return (EXIT_SUCCESS);
}

int create_pipes(t_execute *execute, int *check)
{
    execute->pipe_fds = malloc(2 * execute->num_pipes * sizeof(int));
    if (!execute->pipe_fds)
    {
        *check = 1;
        return (EXIT_FAILURE);
    }
    execute->i = 0;
    while ((int)execute->i < execute->num_pipes)
    {
        if (pipe(execute->pipe_fds + execute->i * 2) < 0)
        {
            *check = 1;
            return (EXIT_FAILURE);
        }
        execute->i++;
    }
    return (EXIT_SUCCESS);
}
int is_directory(const char *path)
{
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}
char *resolve_command_path(char *command, t_execute *execute, t_main *main)
{
    char *path_copy;
    char *path_token;
    char *full_path;
    char *path;

    if (!command || !main->env_list)
    {
        free_execute(execute, 0);
        free_resources(main, 1);
        *execute->exit_status = 127;
        exit(*execute->exit_status);
    }
    if (command[0] == '\0')
    {
        *execute->exit_status = 127;
        return NULL;
    }
    if (command[0] == '/' || command[0] == '.')
    {
        if (access(command, F_OK) == 0)
        {
            if (access(command, X_OK) == 0)
            {
                if (is_directory(command))
                    return (*execute->exit_status = 126, ft_strdup(command));
                return (ft_strdup(command));
            }
            else
                return (*execute->exit_status = 126, NULL);
        }
        return (*execute->exit_status = 127, NULL);
    }
    path = find_env_value(main->env_list, "PATH");
    if (!path)
        return (NULL);
    path_copy = ft_strdup(path);
    if (!path_copy)
    {
        perror("strdup");
        free_execute(execute, 0);
        free_resources(main, 1);
        exit(EXIT_FAILURE);
    }
    path_token = ft_strtok(path_copy, ":");
    while (path_token)
    {
        full_path = malloc(ft_strlen(path_token) + ft_strlen(command) + 2);
        if (!full_path)
        {
            free(path_copy);
            perror("malloc");
            free_execute(execute, 0);
            free_resources(main, 1);
            exit(EXIT_FAILURE);
        }
        ft_strlcpy(full_path, path_token, ft_strlen(path_token) + 1);
        ft_strlcat(full_path, "/", ft_strlen(path_token) + 2);
        ft_strlcat(full_path, command, ft_strlen(path_token) + ft_strlen(command) + 2);
        if (access(full_path, F_OK) == 0)
        {
            if (access(full_path, X_OK) == 0)
            {
                if (is_directory(full_path))
                {
                    free(path_copy);
                    return (*execute->exit_status = 126, full_path);
                }
                free(path_copy);
                return (full_path);
            }
            else
            {
                free(path_copy);
                return (*execute->exit_status = 126, NULL);
            }
        }
        free(full_path);
        path_token = ft_strtok(NULL, ":");
    }
    free(path_copy);
    return (*execute->exit_status = 127, NULL);
}
int is_commands(t_execute *execute, int flag)
{
    char *customs[] = {"echo", "env", "pwd", "exit", "export", "unset", "cd"};
    int i;
    int max;

    i = 0;
    max = 4;
    if (flag == 1)
    {
        i = 3;
        max = 7;
    }
    else if (flag == 2)
    {
        i = 0;
        max = 7;
    }
    while (i < max)
    {
        if (execute->commands[execute->i][0] && ft_strcmp(execute->commands[execute->i][0], customs[i]) == 0)
            return (EXIT_SUCCESS);
        i++;
    }
    return (EXIT_FAILURE);
}

void execute_builtin(t_execute *execute, t_main *main)
{
    *execute->exit_status = 0;
    if (ft_strcmp(execute->commands[execute->i][0], "echo") == 0)
        echo(execute->commands[execute->i]);
    else if (ft_strcmp(execute->commands[execute->i][0], "exit") == 0)
        *execute->exit_status = ft_exit(execute->commands[execute->i]);
    else if (ft_strcmp(execute->commands[execute->i][0], "env") == 0)
        env(execute->envp);
    else if (ft_strcmp(execute->commands[execute->i][0], "pwd") == 0)
        pwd();
    else if (ft_strcmp(execute->commands[execute->i][0], "cd") == 0)
        *execute->exit_status = cd(execute->commands[execute->i],
                                   execute->cmd_args[execute->i], &main->env_list);
    else if (ft_strcmp(execute->commands[execute->i][0], "export") == 0)
        *execute->exit_status = export(execute->commands[execute->i],
                                       execute->cmd_args[execute->i], &main->env_list);
    else if (ft_strcmp(execute->commands[execute->i][0], "unset") == 0)
        *execute->exit_status = unset(execute->commands[execute->i],
                                      &main->env_list);
}

void execute_command(t_execute *execute, t_main *main)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    if (execute->i != 0 && dup2(execute->pipe_fds[(execute->i - 1) * 2], STDIN_FILENO) < 0)
    {
        free_execute(execute, 0);
        free_resources(main, 1);
        // free(main->tokens_list->value);
        exit(EXIT_FAILURE);
    }
    if (execute->i != execute->num_cmds - 1 && dup2(execute->pipe_fds[execute->i * 2 + 1], STDOUT_FILENO) < 0)
    {
        free_execute(execute, 0);
        free_resources(main, 1);
        // free(tokens->value);
        exit(EXIT_FAILURE);
    }
    execute->j = -1;
    while (++execute->j < 2 * execute->num_pipes)
        close(execute->pipe_fds[execute->j]);
    handle_redirections(execute, main);
    if (!is_commands(execute, 2))
    {
        execute_builtin(execute, main);
        free_execute(execute, 0);
        free_resources(main, 1);
        exit(*execute->exit_status);
    }
    execute->cmd_path = resolve_command_path(execute->commands[execute->i][0], execute, main);
    if (!execute->cmd_path)
    {
        if (*execute->exit_status == 126)
            ft_dprintf(2, "Error404: %s: Is a directory\n", execute->commands[execute->i][0]);
        else
            ft_dprintf(2, "Error404: %s: command not found\n", execute->commands[execute->i][0]);
        free_execute(execute, 0);
        free_resources(main, 1);
        exit(*execute->exit_status);
    }
    if (execve(execute->cmd_path, execute->commands[execute->i], execute->envp))
    {
        if (errno == ENOENT)
        {
            ft_dprintf(2, "Error404: command not found");
            *execute->exit_status = 127;
        }
        else
        {
            ft_dprintf(2, "Error404: %s: %s\n", execute->commands[execute->i][0], strerror(errno));
            *execute->exit_status = 126;
        }
        free_execute(execute, 0);
        free_resources(main, 1);
        exit(*execute->exit_status);
    }
}

int handle_builtins(t_execute *execute, t_main *main)
{
    if (ft_strcmp(execute->commands[execute->i][0], "cd") == 0)
        *execute->exit_status = cd(execute->commands[execute->i],
                                   execute->cmd_args[execute->i], &main->env_list);
    else if (ft_strcmp(execute->commands[execute->i][0], "export") == 0)
        *execute->exit_status = export(execute->commands[execute->i],
                                       execute->cmd_args[execute->i], &main->env_list);
    else if (ft_strcmp(execute->commands[execute->i][0], "unset") == 0)
        *execute->exit_status = unset(execute->commands[execute->i],
                                      &main->env_list);
    else if (ft_strcmp(execute->commands[execute->i][0], "exit") == 0)
    {
        *execute->exit_status = ft_exit(execute->commands[execute->i]);
        free_execute(execute, 0);
        free_resources(main, 1);
        exit(*execute->exit_status);
    }
    return (EXIT_SUCCESS);
}

int fork_and_execute(t_execute *execute, t_main *main)
{
    execute->pids = ft_calloc(execute->num_cmds, sizeof(pid_t));
    if (!execute->pids)
        return (EXIT_FAILURE);
    execute->i = 0;
    while (execute->i < execute->num_cmds)
    {
        if (execute->num_cmds == 1 && is_commands(execute, 1) == EXIT_SUCCESS)
        {
            handle_builtins(execute, main);
            execute->i++;
            continue;
        }
        execute->pid = fork();
        if (execute->pid < 0)
        {
            free_execute(execute, 1);
            main->exit_status = 1;
            return (EXIT_FAILURE);
        }
        if (execute->pid == 0)
            execute_command(execute, main);
        else
            execute->pids[execute->i] = execute->pid;
        execute->i++;
    }
    return (EXIT_SUCCESS);
}

void close_pipes_and_wait(t_execute *execute)
{
    int status;
    pid_t pid;
    pid_t last_pid;

    if (!execute->pids)
        return;

    last_pid = execute->pids[execute->num_cmds - 1];
    execute->i = 0;
    while (execute->i < (size_t)(2 * execute->num_pipes))
        close(execute->pipe_fds[execute->i++]);
    while ((pid = wait(&status)) != -1)
    {
        if (pid == last_pid)
        {
            if ((status & 0xFF) == 0)
                *execute->exit_status = (status >> 8) & 0xFF;
            else
                *execute->exit_status = 128 + (status & 0x7F);
            // free_execute(execute, 1);
        }
    }
}
// int loop_check_heredoc(t_redirections *redirections, t_token *token)
// {
//     while (redirections->argv[redirections->j])
//     {
//         if (ft_strcmp(redirections->argv[redirections->j], "<<") == 0
//         && token[redirections->j].type == TOKEN_REDIRECTION_HEREDOC)
//         {
//             signal(SIGINT, handle_heredoc_sigint);
//             if (g_signal == 130)
//                 break;
//             if (redirection_check_else_if(redirections) != EXIT_SUCCESS)
//                 return (EXIT_FAILURE);
//         }
//         else
//             redirections->j++;
//     }
//     return (EXIT_SUCCESS);
// }

// int preprocess_heredocs(t_execute *execute, t_token *token)
// {
//     t_redirections redirections;
//     size_t i;
//     int tmp;

//     tmp = dup(STDIN_FILENO);
//     i = -1;
//     while (++i < execute->num_cmds)
//     {
//         ft_bzero(&redirections, sizeof(t_redirections));
//         redirections.argv = execute->commands[i];
//         redirections.j = 0;
//         if (loop_check_heredoc(&redirections, token) == EXIT_FAILURE)
//             return (EXIT_FAILURE);
//         if (g_signal == 130)
//         {
//             dup2(tmp, STDIN_FILENO);
//             close(tmp);
//             signal(SIGINT, handle_sigint);
//             return (EXIT_FAILURE);
//         }
//     }
//     close(tmp);
//     signal(SIGINT, handle_sigint);
//     redirections.argv = NULL;
//     return (EXIT_SUCCESS);
// }
int preprocess_heredocs(t_execute *execute, t_main *main)
{
    t_redirections redirections;
    size_t i;
    int num_command;
    int tmp = dup(STDIN_FILENO);
    if (tmp < 0)
    {
        perror("dup");
        return (EXIT_FAILURE);
    }
    i = -1;
    num_command = 0;
    while (++i < execute->num_cmds)
    {
        ft_bzero(&redirections, sizeof(t_redirections));
        redirections.argv = execute->commands[i];
        redirections.j = 0;
        while (redirections.argv[redirections.j])
        {
            if (main->tokens_list[num_command].value && main->tokens_list[num_command].type == TOKEN_PIPE)
                num_command++;
            if (ft_strcmp(redirections.argv[redirections.j], "<<") == 0 && main->tokens_list[num_command].type != TOKEN_ARGUMENT)
            {
                signal(SIGINT, handle_heredoc_sigint);
                if (redirection_check_else_if(&redirections, execute, main) != EXIT_SUCCESS)
                    return (EXIT_FAILURE);
                if (g_signal == 130)
                {
                    free_redirections(&redirections);
                    free_execute(execute, 0);
                    break;
                }
            }
            else
            {
                redirections.j++;
                num_command++;
            }
        }
        if (g_signal == 130)
        {
            dup2(tmp, STDIN_FILENO);
            close(tmp);
            signal(SIGINT, handle_sigint);
            return (EXIT_FAILURE);
        }
    }
    close(tmp);
    signal(SIGINT, handle_sigint);
    return (EXIT_SUCCESS);
}

void start_execution(t_main *main)
{
    t_execute execute;
    int check_pipes = 0;
    ft_bzero(&execute, sizeof(t_execute));
    execute.envp = convert_env_to_list(main->env_list);
    execute.exit_status = &main->exit_status;
    *execute.exit_status = 0;
    execute.num_cmds = calculate_number_operations(main->tokens_list, main->cmd->arg_count) + 1;
    execute.num_pipes = execute.num_cmds - 1;
    if (fill_commands(main->tokens_list, main->cmd->arg_count, &execute) || !execute.cmd_args)
    {
        main->exit_status = 1;
        return;
    }
    if (preprocess_heredocs(&execute, main) != EXIT_SUCCESS)
    {
        main->exit_status = 130;
        return;
    }

    if (create_pipes(&execute, &check_pipes))
    {
        main->exit_status = 1;
        return;
    }

    if (fork_and_execute(&execute, main))
    {
        main->exit_status = 1;
        return;
    }
    close_pipes_and_wait(&execute);
    free_execute(&execute, 0);
    return;
}