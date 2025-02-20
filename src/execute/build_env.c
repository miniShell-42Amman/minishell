#include "minishell.h"

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
    while(++execute->i < token_count)
    {
        if(token[execute->i].type == TOKEN_PIPE)
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
    if(init_command(token, token_count, execute))
        return (EXIT_FAILURE);
    execute->i = -1;
    execute->cmd_index = 0;
    execute->arg_index = 0;
    while(++execute->i < token_count)
        {
            if(token[execute->i].type == TOKEN_PIPE)
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

char *resolve_command_path(char *command, t_env *env_list)
{
    char *path_copy;
    char *path_token;
    char *full_path;
    char *path;

    if(command[0] == '/' || command[0] == '.')
        return (ft_strdup(command));
    path = find_env_value(env_list, "PATH");
    if (!path)
        return (NULL);
    path_copy = ft_strdup(path);
    if (!path_copy)
    {
        perror("strdup");
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
            exit(EXIT_FAILURE);
        }
        ft_strlcpy(full_path, path_token, ft_strlen(path_token) + 1);
        ft_strlcat(full_path, "/", ft_strlen(path_token) + 2);
        ft_strlcat(full_path, command, ft_strlen(path_token) + ft_strlen(command) + 2);
        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return (full_path);
        }
        free(full_path);
        path_token = ft_strtok(NULL, ":");
    }
    free(path_copy);
    return (NULL);
}

void execute_command(t_execute *execute)
{
    if (execute->i != 0 && dup2(execute->pipe_fds[(execute->i - 1) * 2], STDIN_FILENO) < 0)
    {
        perror("dup2");
        exit(EXIT_FAILURE);
    }
    if (execute->i != execute->num_cmds - 1 && dup2(execute->pipe_fds[execute->i * 2 + 1], STDOUT_FILENO) < 0)
    {
        perror("dup2");
        exit(EXIT_FAILURE);
    }
    execute->j = -1;
    while (++execute->j < 2 * execute->num_pipes)
        close(execute->pipe_fds[execute->j]);
    execute->cmd_path = resolve_command_path(execute->commands[execute->i][0], execute->env_list);
    if (!execute->cmd_path)
    {
        free_execute(execute);
        exit(EXIT_FAILURE);
    }
    if (execve(execute->cmd_path, execute->commands[execute->i], execute->envp) < 0)
    {
        perror("execve");
        exit(EXIT_FAILURE);
    }
}

int fork_and_execute(t_execute *execute, int *check)
{
    execute->pids = malloc(execute->num_cmds * sizeof(pid_t));
    if (!execute->pids)
        return (EXIT_FAILURE);
    execute->i = 0;
    while (execute->i < execute->num_cmds)
    {
        execute->pid = fork();
        if (execute->pid < 0)
        {
            *check = 1;
            return(EXIT_FAILURE);
        }
        if (execute->pid == 0)
            execute_command(execute);
        else
            execute->pids[execute->i] = execute->pid;
        execute->i++;
    }
    return (EXIT_SUCCESS);
}

void close_pipes_and_wait(t_execute *execute)
{
    execute->i = 0;
    while ((int)execute->i < 2 * execute->num_pipes)
    {
        close(execute->pipe_fds[execute->i]);
        execute->i++;
    }
    execute->i = 0;
    while (execute->i < execute->num_cmds)
    {
        waitpid(execute->pids[execute->i], NULL, 0);
        execute->i++;
    }
}

void start_execution(t_token *tokens, size_t token_count, t_env *env_list)
{
    t_execute execute;
    int check_pipes = 0;
    ft_bzero(&execute, sizeof(t_execute));
    execute.env_list = env_list;
    execute.envp = convert_env_to_list(env_list);
    execute.num_cmds = calculate_number_operations(tokens, token_count) + 1;
    execute.num_pipes = execute.num_cmds - 1;
    if (fill_commands(tokens, token_count, &execute) || !execute.cmd_args
        || create_pipes(&execute,&check_pipes) || fork_and_execute(&execute,&check_pipes))
    {
        free_execute(&execute);
        if(check_pipes)
            close_pipes_and_wait(&execute);
        return;
    }
    close_pipes_and_wait(&execute);
    free_execute(&execute);
}
