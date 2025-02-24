#include "minishell.h"

char *append_str(char *dest, size_t *dest_size, const char *src)
{
    size_t src_len = ft_strlen(src);
    char *new_ptr = realloc(dest, *dest_size + src_len + 1);
    if (!new_ptr)
    {
        free(dest);
        perror("Erorr404: realloc");
        exit(EXIT_FAILURE);
    }
    ft_memcpy(new_ptr + *dest_size, src, src_len + 1);
    *dest_size += src_len;
    return new_ptr;
}

void choose_flags_fd(t_redirections *redirections, int *flags, int *fd, int *std_fd)
{
    *std_fd = 0;
    *flags = 0;
    if (ft_strcmp(redirections->op, ">") == 0)
        *flags = O_WRONLY | O_CREAT | O_TRUNC;
    else if (ft_strcmp(redirections->op, ">>") == 0)
        *flags = O_WRONLY | O_CREAT | O_APPEND;
    else if (ft_strcmp(redirections->op, "<") == 0)
        *flags = O_RDONLY;
    *fd = open(redirections->target, *flags, 0644);
    if (*fd < 0)
    {
        perror("Erorr404: open");
        exit(EXIT_FAILURE);
    }
    if (*flags == O_RDONLY)
        *std_fd = STDIN_FILENO;
    else
        *std_fd = STDOUT_FILENO;
}

int redirection_check(t_redirections *redirections)
{
    int fd;
    int flags;
    int std_fd;

    redirections->op = redirections->argv[redirections->j];
    redirections->target = redirections->argv[redirections->j + 1];
    if (!redirections->target && ft_dprintf(STDERR_FILENO, "Erorr404: syntax error near token `%s'\n", redirections->op))
        exit(EXIT_FAILURE);
    choose_flags_fd(redirections, &flags, &fd, &std_fd);
    if (dup2(fd, std_fd) < 0)
        exit(EXIT_FAILURE);
    close(fd);
    redirections->k = redirections->j;
    while (redirections->argv[redirections->k + 2])
    {
        redirections->argv[redirections->k] = redirections->argv[redirections->k + 2];
        redirections->k++;
    }
    redirections->argv[redirections->k] = NULL;
    return (EXIT_SUCCESS);
}
// void handle_heredoc_sigint(int signum)
// {
//     (void)signum;

//     g_signal = 130;
//     exit(STDIN_FILENO);
// }

// int redirection_check_else_if(t_redirections *redirections)
// {
//     struct sigaction sa_orig, sa_new;

//     char *target = redirections->argv[redirections->j + 1];
//     if (!target)
//     {
//         ft_dprintf(STDERR_FILENO, "Erorr404: syntax error near token `<<'\n");
//         // exit(EXIT_FAILURE);
//         return (EXIT_FAILURE);
//     }

//     char *current_doc = NULL;
//     size_t current_size = 0;
//     char *line;
//     sigemptyset(&sa_new.sa_mask);
//     sa_new.sa_handler = handle_heredoc_sigint;
//     // sa_new.sa_flags = 0;
//     sigaction(SIGINT, &sa_new, &sa_orig);
//     while (1)
//     {
//         // line = readline("> ");
//         if (g_signal == 130)
//         {
//             ft_dprintf(STDERR_FILENO, "\n");
//             free(line);
//             free(current_doc);
//             redirections->argv[redirections->j] = NULL;
//             sigaction(SIGINT, &sa_orig, NULL);
//             g_signal = 0;
//             return (EXIT_FAILURE);
//         }
//         line = readline("> ");
//         if (!line)
//         {
//             ft_dprintf(STDERR_FILENO, "Erorr404: warning: here-document delimited by EOF\n");
//             break;
//         }
//         if (ft_strcmp(line, target) == 0)
//         {
//             free(line);
//             break;
//         }

//         current_doc = append_str(current_doc, &current_size, line);
//         current_doc = append_str(current_doc, &current_size, "\n");
//         free(line);
//     }
//     sigaction(SIGINT, &sa_orig, NULL);
//     if (current_doc)
//     {
//         redirections->heredoc_all = append_str(redirections->heredoc_all,
//                                                &redirections->heredoc_total_size,
//                                                current_doc);
//         free(current_doc);
//     }

//     redirections->k = redirections->j;
//     while (redirections->argv[redirections->k + 2])
//     {
//         redirections->argv[redirections->k] = redirections->argv[redirections->k + 2];
//         redirections->k++;
//     }
//     redirections->argv[redirections->k] = NULL;
//     return (EXIT_SUCCESS);
// }

void handle_heredoc_sigint(int signum)
{
    (void)signum;
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    // rl_redisplay();
    g_signal = 130;
}

int redirection_check_else_if(t_redirections *redirections)
{
    // (void)flage;
    // struct sigaction sa_orig, sa_new;

    char *target = redirections->argv[redirections->j + 1];
    if (!target)
    {
        ft_dprintf(STDERR_FILENO, "Erorr404: syntax error near token `<<'\n");
        return (EXIT_FAILURE);
    }

    char *current_doc = NULL;
    size_t current_size = 0;
    char *line;
    signal(SIGINT, handle_heredoc_sigint);
    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            ft_dprintf(STDERR_FILENO, "Erorr404: warning: here-document delimited by EOF\n");
            break;
        }
        if (ft_strcmp(line, target) == 0)
        {
            free(line);
            break;
        }

        current_doc = append_str(current_doc, &current_size, line);
        current_doc = append_str(current_doc, &current_size, "\n");
        free(line);
    }
    // if (flage ==1 )
    //     sigaction(SIGINT, &sa_orig, NULL);
    if (current_doc)
    {
        redirections->heredoc_all = append_str(redirections->heredoc_all,
                                               &redirections->heredoc_total_size,
                                               current_doc);
        free(current_doc);
    }

    redirections->k = redirections->j;
    while (redirections->argv[redirections->k + 2])
    {
        redirections->argv[redirections->k] = redirections->argv[redirections->k + 2];
        redirections->k++;
    }
    redirections->argv[redirections->k] = NULL;
    return (EXIT_SUCCESS);
}
void if_redirections_heredoc_all(t_redirections *redirections)
{
    if (redirections->heredoc_all)
    {
        int pipefd[2];
        if (pipe(pipefd) < 0)
        {
            perror("Erorr404: pipe");
            exit(EXIT_FAILURE);
        }
        if (write(pipefd[1], redirections->heredoc_all, redirections->heredoc_total_size) < 0)
        {
            perror("Erorr404: write");
            exit(EXIT_FAILURE);
        }
        free(redirections->heredoc_all);
        close(pipefd[1]);
        if (dup2(pipefd[0], STDIN_FILENO) < 0)
        {
            perror("Erorr404: dup2");
            exit(EXIT_FAILURE);
        }
        close(pipefd[0]);
    }
}
void handle_redirections(t_execute *execute)
{
    t_redirections redirections;

    ft_bzero(&redirections, sizeof(t_redirections));
    redirections.argv = execute->commands[execute->i];

    while (redirections.argv[redirections.j])
    {
        if ((!ft_strcmp(redirections.argv[redirections.j], ">") ||
             !ft_strcmp(redirections.argv[redirections.j], ">>") ||
             !ft_strcmp(redirections.argv[redirections.j], "<")) &&
            !redirection_check(&redirections))
            continue;
        else if (!ft_strcmp(redirections.argv[redirections.j], "<<") &&
                 !redirection_check_else_if(&redirections))
            continue;
        redirections.j++;
    }
    if_redirections_heredoc_all(&redirections);
}
