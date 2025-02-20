#include "minishell.h"

// int is_delim(char c, const char *delim)
// {
//     while (*delim)
//     {
//         if (c == *delim)
//             return 1;
//         delim++;
//     }
//     return 0;
// }
// char *ft_strtok(char *str, const char *delim)
// {
//     static char *last = NULL;
//     if (str)
//         last = str;
//     if (!last)
//         return NULL;
//     char *start = last;
//     while (*last)
//     {
//         if (is_delim(*last, delim))
//         {
//             *last = '\0';
//             last++;
//             if (start == last)
//             {
//                 start++;
//                 continue;
//             }
//             return start;
//         }
//         last++;
//     }
//     last = NULL;
//     if (start == last)
//         return NULL;
//     return start;
// }


// char *resolve_command_path(char *command, t_env *env_list)
// {
//     if (command[0] == '/' || command[0] == '.')
//     {
//         return ft_strdup(command);
//     }
//     char *path = find_env_value(env_list, "PATH");
//     if (!path)
//     {
//         return NULL;
//     }
//     char *path_copy = ft_strdup(path);
//     if (!path_copy)
//     {
//         perror("strdup");
//         exit(EXIT_FAILURE);
//     }
//     // char *path_token = strtok(path_copy, ":");
//     char *path_token = ft_strtok(path_copy, ":");
//     while (path_token)
//     {

//         char *full_path = malloc(ft_strlen(path_token) + ft_strlen(command) + 2);
//         if (!full_path)
//         {
//             free(path_copy);
//             perror("malloc");
//             exit(EXIT_FAILURE);
//         }
//         // strcpy(full_path, path_token);
//         ft_strlcpy(full_path, path_token, ft_strlen(path_token) + 1);
//         // strcat(full_path, "/");
//         ft_strlcat(full_path, "/", ft_strlen(path_token) + 2);

//         // strcat(full_path, command);
//         ft_strlcat(full_path, command, ft_strlen(path_token) + ft_strlen(command) + 2);
//         if (access(full_path, X_OK) == 0)
//         {
//             free(path_copy);
//             return full_path;
//         }
//         free(full_path);
//         // path_token = strtok(NULL, ":");
//         path_token = ft_strtok(NULL, ":");
//     }
//     free(path_copy);
//     return NULL;
// }

// char **build_env_array(t_env *env_list)
// {
//     if (!env_list)
//     {
//         return NULL;
//     }
//     int env_count = 0;
//     t_env *current = env_list;
//     while (current)
//     {
//         env_count++;
//         current = current->next;
//     }
//     char **envp = malloc((env_count + 1) * sizeof(char *));
//     if (!envp)
//     {
//         perror("malloc");
//         exit(EXIT_FAILURE);
//     }
//     current = env_list;
//     for (int i = 0; i < env_count; i++)
//     {
//         size_t key_len = ft_strlen(current->key);
//         size_t value_len = (current->value != NULL) ? ft_strlen(current->value) : 0;
//         char *env_var = malloc(key_len + value_len + 2);
//         if (!env_var)
//         {
//             perror("malloc");
//             for (int j = 0; j < i; j++)
//                 free(envp[j]);
//             free(envp);
//             exit(EXIT_FAILURE);
//         }
//         ft_strlcpy(env_var, current->key, key_len + 1);
//         ft_strlcat(env_var, "=", key_len + 2);
//         if (current->value)
//             ft_strlcat(env_var, current->value, key_len + value_len + 2);
//         envp[i] = env_var;
//         current = current->next;
//     }
//     envp[env_count] = NULL;
//     //   printf("=== Environment Variables ===\n");
//     // for (int i = 0; envp[i]; i++)
//     // {
//     //     // printf("envp[%d]: %s\n", i, envp[i]);
//     // }
//     // printf("=============================\n");

//     return envp;
// }

// void execute_tokens_pipeline(t_token *tokens, int token_count, t_env *env_list)
// {
   
//     char **envp = build_env_array(env_list);

//     // int num_cmds = 1;
//     // for (int i = 0; i < token_count; i++)
//     // {
//     //     if (tokens[i].type == TOKEN_PIPE)
//     //         num_cmds++;
//     // }

//     // 2. تقسيم التوكنات إلى أوامر (كل أمر عبارة عن مصفوفة من سلاسل نصية (char **))
//     char ***commands = malloc(num_cmds * sizeof(char **));
//     int *cmd_argc = malloc(num_cmds * sizeof(int));
//     if (!commands || !cmd_argc)
//     {
//         perror("malloc");
//         free(envp);
//         exit(EXIT_FAILURE);
//     }
//     for (int i = 0; i < num_cmds; i++)
//     {
//         commands[i] = NULL;
//         cmd_argc[i] = 0;
//     }

//     // أولاً، نحسب عدد التوكنات (غير PIPE) لكل أمر
//     int cmd_index = 0;
//     for (int i = 0; i < token_count; i++)
//     {
//         if (tokens[i].type == TOKEN_PIPE)
//         {
//             cmd_index++;
//         }
//         else
//         {
//             cmd_argc[cmd_index]++;
//         }
//     }

//     // 3. نخصص لكل أمر مصفوفة من السلاسل (مع نهاية NULL)
//     for (int i = 0; i < num_cmds; i++)
//     {
//         commands[i] = malloc((cmd_argc[i] + 1) * sizeof(char *));
//         if (!commands[i])
//         {
//             perror("malloc");
//             for (int j = 0; j < i; j++)
//                 free(commands[j]);
//             free(commands);
//             free(cmd_argc);
//             free(envp);
//             exit(EXIT_FAILURE);
//         }
//         commands[i][cmd_argc[i]] = NULL; // نهاية المصفوفة
//     }

//     // 4. نملأ مصفوفات الأوامر بالقيم من التوكنات
//     cmd_index = 0;
//     int arg_index = 0;
//     for (int i = 0; i < token_count; i++)
//     {
//         if (tokens[i].type == TOKEN_PIPE)
//         {
//             cmd_index++;
//             arg_index = 0;
//         }
//         else
//         {
//             commands[cmd_index][arg_index] = ft_strdup(tokens[i].value);
//             if (!commands[cmd_index][arg_index])
//             {
//                 perror("strdup");
//                 for (int j = 0; j <= cmd_index; j++)
//                 {
//                     int max = (j == cmd_index) ? arg_index : cmd_argc[j];
//                     for (int k = 0; k < max; k++)
//                         free(commands[j][k]);
//                     free(commands[j]);
//                 }
//                 free(commands);
//                 free(cmd_argc);
//                 for (int j = 0; envp[j]; j++)
//                     free(envp[j]);
//                 free(envp);
//                 exit(EXIT_FAILURE);
//             }
//             arg_index++;
//         }
//     }

//     // 5. إعداد الأنابيب (عدد الأنابيب = num_cmds - 1)
//     int num_pipes = num_cmds - 1;
//     int pipefds[2 * num_pipes];
//     for (int i = 0; i < num_pipes; i++)
//     {
//         if (pipe(pipefds + i * 2) < 0)
//         {
//             perror("pipe");
//             exit(EXIT_FAILURE);
//         }
//     }

//     // 6. تنفيذ كل أمر بعملية fork
//     pid_t *pids = malloc(num_cmds * sizeof(pid_t));
//     if (!pids)
//     {
//         perror("malloc");
//         exit(EXIT_FAILURE);
//     }
//     for (int i = 0; i < num_cmds; i++)
//     {
//         pid_t pid = fork();
//         if (pid < 0)
//         {
//             perror("fork");
//             exit(EXIT_FAILURE);
//         }
//         if (pid == 0)
//         {
//             // عملية الطفل:
//             // إذا ليس أول أمر، وجه STDIN إلى نهاية القراءة من الأنبوب السابق
//             if (i != 0)
//             {
//                 if (dup2(pipefds[(i - 1) * 2], STDIN_FILENO) < 0)
//                 {
//                     perror("dup2");
//                     exit(EXIT_FAILURE);
//                 }
//             }
//             // إذا ليس آخر أمر، وجه STDOUT إلى نهاية الكتابة من الأنبوب الحالي
//             if (i != num_cmds - 1)
//             {
//                 if (dup2(pipefds[i * 2 + 1], STDOUT_FILENO) < 0)
//                 {
//                     perror("dup2");
//                     exit(EXIT_FAILURE);
//                 }
//             }
//             // إغلاق الأنابيب غير المستخدمة
//             for (int j = 0; j < 2 * num_pipes; j++)
//             {
//                 close(pipefds[j]);
//             }
//             // تنفيذ الأمر
//             char *cmd_path = resolve_command_path(commands[i][0], env_list);
//             if (!cmd_path)
//             {
//                 // fprintf(stderr, "Command not found: %s\n", commands[i][0]);
//                 ft_dprintf(2, "Command not found: %s\n", commands[i][0]);
//                 free(commands);
//                 free(cmd_argc);
//                 free(pids);
//                 for (int i = 0; envp[i]; i++)
//                 {
//                     free(envp[i]);
//                 }
//                 free(envp);
//                 exit(EXIT_FAILURE);
//             }
//             if (execve(cmd_path, commands[i], envp) < 0)
//             {
//                 perror("execve");
//                 free(commands);
//                 free(cmd_argc);
//                 free(pids);
//                 for (int i = 0; envp[i]; i++)
//                 {
//                     free(envp[i]);
//                 }
//                 free(envp);
//                 exit(EXIT_FAILURE);
//             }
//         }
//         else
//         {
//             // في عملية الأب نخزن PID
//             pids[i] = pid;
//         }
//     }

//     // إغلاق الأنابيب في الأب
//     for (int i = 0; i < 2 * num_pipes; i++)
//     {
//         close(pipefds[i]);
//     }

//     // انتظار انتهاء جميع العمليات الفرعية
//     for (int i = 0; i < num_cmds; i++)
//     {
//         waitpid(pids[i], NULL, 0);
//     }

//     // تحرير الذاكرة المخصصة للأوامر
//     for (int i = 0; i < num_cmds; i++)
//     {
//         for (int j = 0; j < cmd_argc[i]; j++)
//         {
//             free(commands[i][j]);
//         }
//         free(commands[i]);
//     }
//     free(commands);
//     free(cmd_argc);
//     free(pids);
//     for (int i = 0; envp[i]; i++)
//     {
//         free(envp[i]);
//     }
//     free(envp);
// }

int is_delim(char c, const char *delim)
{
    while (*delim)
    {
        if (c == *delim)
            return 1;
        delim++;
    }
    return 0;
}
char *ft_strtok(char *str, const char *delim)
{
    static char *last = NULL;
    if (str)
        last = str;
    if (!last)
        return NULL;
    char *start = last;
    while (*last)
    {
        if (is_delim(*last, delim))
        {
            *last = '\0';
            last++;
            if (start == last)
            {
                start++;
                continue;
            }
            return start;
        }
        last++;
    }
    last = NULL;
    if (start == last)
        return NULL;
    return start;
}

int	ft_list_size(t_env *env_list)
{
    if (env_list == NULL)
        return (0);
    else if (env_list->next == NULL)
        return (1);
    else
        return (1 + ft_list_size(env_list->next));
}

int fill_env_list(char ***envp, t_env *env_list, int envp_count)
{
    int i;

    i = 0;
    while (i < envp_count)
    {
        size_t envp_len;

        envp_len = ft_strlen(env_list->key) + ft_strlen(env_list->value) + 2;
        (*envp)[i] = malloc(envp_len);
        if (!(*envp)[i]) 
        {
            perror("malloc");
            ft_free_split(*envp);
            return (EXIT_FAILURE);
        }
        ft_strlcpy((*envp)[i], env_list->key, envp_len);
        ft_strlcat((*envp)[i], "=", envp_len);
        if (env_list->value)
            ft_strlcat((*envp)[i], env_list->value, envp_len);
        i++;
        env_list = env_list->next;
    }
    (*envp)[envp_count] = NULL;
    return (EXIT_SUCCESS);
}

char **convert_env_to_list(t_env *env_list)
{
    int env_count = 0;
    t_env *current = env_list;
    env_count = ft_list_size(current);
    char **envp = malloc((env_count + 1) * sizeof(char *));
    if (!envp)
    {
        perror("malloc");
        return (NULL);    
    }
    if (fill_env_list(&envp, env_list, env_count))
        return (NULL);
    return (envp);
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

void free_execute(t_execute *execute)
{
    execute->i = 0;
    if (execute->envp)
        ft_free_split(execute->envp);
    if (execute->commands)
    {
        while (execute->i < execute->num_cmds)
        {
            if (execute->commands[execute->i])
                ft_free_split(execute->commands[execute->i]);
            execute->i++;
        }
        free(execute->commands);
    }
    if (execute->cmd_args)
    {
        free(execute->cmd_args);
        execute->cmd_args = NULL;
    }
    if (execute->pipe_fds)
    {
        free(execute->pipe_fds);
        execute->pipe_fds = NULL;
    }
    if (execute->pids)
    {
        free(execute->pids);
        execute->pids = NULL;
    }
    if (execute->cmd_path)
    {
        free(execute->cmd_path);
        execute->cmd_path = NULL;
    }
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
int create_pipes(t_execute *execute)
{
    execute->pipe_fds = malloc(2 * execute->num_pipes * sizeof(int));
    if (!execute->pipe_fds)
        return (EXIT_FAILURE);
    // execute->pipe_fds[2 * execute->num_pipes];
    execute->i = 0;
    while ((int)execute->i < execute->num_pipes)
    {
        if (pipe(execute->pipe_fds + execute->i * 2) < 0)
            return (EXIT_FAILURE);   
        execute->i++;
    }
    return (EXIT_SUCCESS);
}

char *resolve_command_path(char *command, t_env *env_list)
{
    char *path_copy;
    char *path_token;
    char *full_path;
    if(command[0] == '/' || command[0] == '.')
        return ft_strdup(command);
    char *path = find_env_value(env_list, "PATH");
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
            return full_path;
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
    int j = 0;
    while (j < 2 * execute->num_pipes)
    {
        close(execute->pipe_fds[j]);
        j++;
    }
    execute->cmd_path = resolve_command_path(execute->commands[execute->i][0], execute->env_list);
if (!execute->cmd_path)
{
    ft_dprintf(2, "Error: Command not found: %s\n", execute->commands[execute->i][0]);
    free_execute(execute);
    exit(EXIT_FAILURE);
}
    if (execve(execute->cmd_path, execute->commands[execute->i], execute->envp) < 0)
    {
        perror("execve");
        exit(EXIT_FAILURE);
    }
}

int fork_and_execute(t_execute *execute)
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
            free(execute->pids);
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

    ft_bzero(&execute, sizeof(t_execute));
    execute.env_list = env_list;
    execute.envp = convert_env_to_list(env_list);
    execute.num_cmds = calculate_number_operations(tokens, token_count) + 1;
    execute.num_pipes = execute.num_cmds - 1;
    if (fill_commands(tokens, token_count, &execute) || !execute.cmd_args
        || create_pipes(&execute) || fork_and_execute(&execute))
    {
        free_execute(&execute);
        return;
    }
    close_pipes_and_wait(&execute);
    free_execute(&execute);
}

