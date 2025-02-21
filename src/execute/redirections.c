#include "minishell.h"

void handle_redirections(t_execute *execute)
{
    char **argv = execute->commands[execute->i];
    int j = 0, k;

    while (argv[j])
    {
        if (ft_strcmp(argv[j], ">") == 0 ||
            ft_strcmp(argv[j], ">>") == 0 ||
            ft_strcmp(argv[j], "<") == 0 ||
            ft_strcmp(argv[j], "<<") == 0)
        {
            char *op = argv[j];
            char *target = argv[j+1];
            if (!target)
            {
                ft_dprintf(STDERR_FILENO, "syntax error near unexpected token `%s'\n", op);
                exit(EXIT_FAILURE);
            }
            if (ft_strcmp(op, ">") == 0)
            {
                int fd = open(target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) { perror("open"); exit(EXIT_FAILURE); }
                if (dup2(fd, STDOUT_FILENO) < 0) { perror("dup2"); exit(EXIT_FAILURE); }
                close(fd);
            }
            else if (ft_strcmp(op, ">>") == 0)
            {
                int fd = open(target, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fd < 0) { perror("open"); exit(EXIT_FAILURE); }
                if (dup2(fd, STDOUT_FILENO) < 0) { perror("dup2"); exit(EXIT_FAILURE); }
                close(fd);
            }
            else if (ft_strcmp(op, "<") == 0)
            {
                int fd = open(target, O_RDONLY);
                if (fd < 0) { perror("open"); exit(EXIT_FAILURE); }
                if (dup2(fd, STDIN_FILENO) < 0) { perror("dup2"); exit(EXIT_FAILURE); }
                close(fd);
            }
            else if (ft_strcmp(op, "<<") == 0)
            {
                int pipefd[2];
                if (pipe(pipefd) < 0) { perror("pipe"); exit(EXIT_FAILURE); }
                char *line = NULL;
                size_t len = 0;
                ft_dprintf(STDOUT_FILENO, "> ");
                while (getline(&line, &len, stdin) != -1)
                {
                    line[strcspn(line, "\n")] = '\0';
                    if (ft_strcmp(line, target) == 0)
                        break;
                    write(pipefd[1], line, strlen(line));
                    write(pipefd[1], "\n", 1);
                    ft_dprintf(STDOUT_FILENO, "> ");
                }
                free(line);
                close(pipefd[1]);
                if (dup2(pipefd[0], STDIN_FILENO) < 0) { perror("dup2"); exit(EXIT_FAILURE); }
                close(pipefd[0]);
            }
            k = j;
            while (argv[k+2])
            {
                argv[k] = argv[k+2];
                k++;
            }
            argv[k] = NULL;
            argv[k+1] = NULL;
        }
        else
        {
            j++;
        }
    }
}

