/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 20:10:32 by oissa             #+#    #+#             */
/*   Updated: 2025/02/09 20:18:05 by oissa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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


char *resolve_command_path(char *command, t_env *env_list)
{
    if (command[0] == '/' || command[0] == '.')
    {
        return ft_strdup(command);
    }
    char *path = find_env_value(env_list, "PATH");
    if (!path)
    {
        return NULL;
    }
    char *path_copy = ft_strdup(path);
    if (!path_copy)
    {
        perror("strdup");
        exit(EXIT_FAILURE);
    }
    // char *path_token = strtok(path_copy, ":");
    char *path_token = ft_strtok(path_copy, ":");
    while (path_token)
    {

        char *full_path = malloc(ft_strlen(path_token) + ft_strlen(command) + 2);
        if (!full_path)
        {
            free(path_copy);
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        // strcpy(full_path, path_token);
        ft_strlcpy(full_path, path_token, ft_strlen(path_token) + 1);
        // strcat(full_path, "/");
        ft_strlcat(full_path, "/", ft_strlen(path_token) + 2);

        // strcat(full_path, command);
        ft_strlcat(full_path, command, ft_strlen(path_token) + ft_strlen(command) + 2);
        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return full_path;
        }
        free(full_path);
        // path_token = strtok(NULL, ":");
        path_token = ft_strtok(NULL, ":");
    }
    free(path_copy);
    return NULL;
}

char **build_env_array(t_env *env_list)
{
    if (!env_list)
    {
        return NULL;
    }
    int env_count = 0;
    t_env *current = env_list;
    while (current)
    {
        env_count++;
        current = current->next;
    }
    char **envp = malloc((env_count + 1) * sizeof(char *));
    if (!envp)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    current = env_list;
    for (int i = 0; i < env_count; i++)
    {
        size_t key_len = ft_strlen(current->key);
        size_t value_len = (current->value != NULL) ? ft_strlen(current->value) : 0;
        char *env_var = malloc(key_len + value_len + 2);
        if (!env_var)
        {
            perror("malloc");
            for (int j = 0; j < i; j++)
                free(envp[j]);
            free(envp);
            exit(EXIT_FAILURE);
        }
        ft_strlcpy(env_var, current->key, key_len + 1);
        ft_strlcat(env_var, "=", key_len + 2);
        if (current->value)
            ft_strlcat(env_var, current->value, key_len + value_len + 2);
        envp[i] = env_var;
        current = current->next;
    }
    envp[env_count] = NULL;
    //   printf("=== Environment Variables ===\n");
    // for (int i = 0; envp[i]; i++)
    // {
    //     // printf("envp[%d]: %s\n", i, envp[i]);
    // }
    // printf("=============================\n");

    return envp;
}

void execute_tokens_pipeline(t_token *tokens, int token_count, t_env *env_list)
{
    if (token_count == 0)
    {
        return;
    }
    if (tokens[0].type == TOKEN_PIPE)
    {
        // fprintf(stderr, "syntax error near unexpected token '|'\n");
        ft_dprintf(2, "syntax error near unexpected token '|'\n");
        return;
    }
    if (tokens[token_count - 1].type == TOKEN_PIPE)
    {
        // fprintf(stderr, "syntax error near unexpected token '|'\n");
        ft_dprintf(2, "syntax error near unexpected token '|'\n");
        return;
    }
    for (int i = 1; i < token_count; i++)
    {
        if (tokens[i].type == TOKEN_PIPE && tokens[i - 1].type == TOKEN_PIPE)
        {
            // fprintf(stderr, "syntax error near unexpected token '|'\n");
            ft_dprintf(2, "syntax error near unexpected token '|'\n");
            return;
        }
    }
    char **envp = build_env_array(env_list);
    int num_cmds = 1;
    for (int i = 0; i < token_count; i++)
    {
        if (tokens[i].type == TOKEN_PIPE)
            num_cmds++;
    }

    // 2. تقسيم التوكنات إلى أوامر (كل أمر عبارة عن مصفوفة من سلاسل نصية (char **))
    char ***commands = malloc(num_cmds * sizeof(char **));
    int *cmd_argc = malloc(num_cmds * sizeof(int));
    if (!commands || !cmd_argc)
    {
        perror("malloc");
        free(envp);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < num_cmds; i++)
    {
        commands[i] = NULL;
        cmd_argc[i] = 0;
    }

    // أولاً، نحسب عدد التوكنات (غير PIPE) لكل أمر
    int cmd_index = 0;
    for (int i = 0; i < token_count; i++)
    {
        if (tokens[i].type == TOKEN_PIPE)
        {
            cmd_index++;
        }
        else
        {
            cmd_argc[cmd_index]++;
        }
    }

    // 3. نخصص لكل أمر مصفوفة من السلاسل (مع نهاية NULL)
    for (int i = 0; i < num_cmds; i++)
    {
        commands[i] = malloc((cmd_argc[i] + 1) * sizeof(char *));
        if (!commands[i])
        {
            perror("malloc");
            for (int j = 0; j < i; j++)
                free(commands[j]);
            free(commands);
            free(cmd_argc);
            free(envp);
            exit(EXIT_FAILURE);
        }
        commands[i][cmd_argc[i]] = NULL; // نهاية المصفوفة
    }

    // 4. نملأ مصفوفات الأوامر بالقيم من التوكنات
    cmd_index = 0;
    int arg_index = 0;
    for (int i = 0; i < token_count; i++)
    {
        if (tokens[i].type == TOKEN_PIPE)
        {
            cmd_index++;
            arg_index = 0;
        }
        else
        {
            commands[cmd_index][arg_index] = ft_strdup(tokens[i].value);
            if (!commands[cmd_index][arg_index])
            {
                perror("strdup");
                for (int j = 0; j <= cmd_index; j++)
                {
                    int max = (j == cmd_index) ? arg_index : cmd_argc[j];
                    for (int k = 0; k < max; k++)
                        free(commands[j][k]);
                    free(commands[j]);
                }
                free(commands);
                free(cmd_argc);
                for (int j = 0; envp[j]; j++)
                    free(envp[j]);
                free(envp);
                exit(EXIT_FAILURE);
            }
            arg_index++;
        }
    }

    // 5. إعداد الأنابيب (عدد الأنابيب = num_cmds - 1)
    int num_pipes = num_cmds - 1;
    int pipefds[2 * num_pipes];
    for (int i = 0; i < num_pipes; i++)
    {
        if (pipe(pipefds + i * 2) < 0)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // 6. تنفيذ كل أمر بعملية fork
    pid_t *pids = malloc(num_cmds * sizeof(pid_t));
    if (!pids)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < num_cmds; i++)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
        {
            // عملية الطفل:
            // إذا ليس أول أمر، وجه STDIN إلى نهاية القراءة من الأنبوب السابق
            if (i != 0)
            {
                if (dup2(pipefds[(i - 1) * 2], STDIN_FILENO) < 0)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }
            // إذا ليس آخر أمر، وجه STDOUT إلى نهاية الكتابة من الأنبوب الحالي
            if (i != num_cmds - 1)
            {
                if (dup2(pipefds[i * 2 + 1], STDOUT_FILENO) < 0)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }
            // إغلاق الأنابيب غير المستخدمة
            for (int j = 0; j < 2 * num_pipes; j++)
            {
                close(pipefds[j]);
            }
            // تنفيذ الأمر
            char *cmd_path = resolve_command_path(commands[i][0], env_list);
            if (!cmd_path)
            {
                // fprintf(stderr, "Command not found: %s\n", commands[i][0]);
                ft_dprintf(2, "Command not found: %s\n", commands[i][0]);
                free(commands);
                free(cmd_argc);
                free(pids);
                for (int i = 0; envp[i]; i++)
                {
                    free(envp[i]);
                }
                free(envp);
                exit(EXIT_FAILURE);
            }
            if (execve(cmd_path, commands[i], envp) < 0)
            {
                perror("execve");
                free(commands);
                free(cmd_argc);
                free(pids);
                for (int i = 0; envp[i]; i++)
                {
                    free(envp[i]);
                }
                free(envp);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            // في عملية الأب نخزن PID
            pids[i] = pid;
        }
    }

    // إغلاق الأنابيب في الأب
    for (int i = 0; i < 2 * num_pipes; i++)
    {
        close(pipefds[i]);
    }

    // انتظار انتهاء جميع العمليات الفرعية
    for (int i = 0; i < num_cmds; i++)
    {
        waitpid(pids[i], NULL, 0);
    }

    // تحرير الذاكرة المخصصة للأوامر
    for (int i = 0; i < num_cmds; i++)
    {
        for (int j = 0; j < cmd_argc[i]; j++)
        {
            free(commands[i][j]);
        }
        free(commands[i]);
    }
    free(commands);
    free(cmd_argc);
    free(pids);
    for (int i = 0; envp[i]; i++)
    {
        free(envp[i]);
    }
    free(envp);
}

int main(int ac, char **av, char **env)
{
    t_main main;
    (void)ac;
    (void)av;
    main.env_list = save_env(env);
    while (1)
    {
        ft_bzero(&main.cmd, sizeof(t_cmd));
        // ft_printf(YELLOW);
        main.input = readline("\001\033[35m\002⚠️  Error404 ⚠️  >\001\033[34m\002 ");
        int *array = ft_count_token(main.input);
        // ft_printf(RESET);
        if (main.input == NULL)
            break;
        if (ft_strcmp(main.input, "exit") == 0)
        {
            free(main.input);
            free_command(&main.cmd);
            free(array);
            // free_env(main.env_list);
            free_env_list(main.env_list);
            rl_clear_history();
            break;
        }
        if (*main.input)
        {
            add_history(main.input);
            main.cmd = parse_cmd(main.input, main.env_list);
            if (main.cmd.args != NULL)
            {
                main.tokens_list = store_token(main.cmd.args, main.cmd.arg_count, array);
                if (main.tokens_list)
                {
                    // for (int k = 0; k < main.cmd.arg_count; k++)
                    
                    // //    if (main.tokens_list[k].value)
                    // //    {
                    // //      if (main.tokens_list[k].value != NULL)
                    // //         printf("Token[%d]: %s => %d\n", k, main.tokens_list[k].value, main.tokens_list[k].type);
                    // //     else
                    // //         printf("Token[%d]: (null) => %d\n", k, main.tokens_list[k].type);
                    // //     free(main.tokens_list[k].value);
                    // //    }
                    //  if (main.tokens_list[k].value)
                    //    printf("Token[%d]: %s => %d\n", k, main.tokens_list[k].value, main.tokens_list[k].type);
                    // }
                    if (main.cmd.arg_count != 0)
                        execute_tokens_pipeline(main.tokens_list, main.cmd.arg_count, main.env_list);
                    // Now free the tokens
                    for (int k = 0; k < main.cmd.arg_count; k++)
                    {
                        free(main.tokens_list[k].value);
                    }
                    free(main.tokens_list);
                }
                free_command(&main.cmd);
            }
        }
        if (array)
            free(array);
        if (main.input)
            free(main.input);
        // free_env_list(main.env_list);
    }
    return 0;
}
