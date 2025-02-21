#include "minishell.h"

/*
  دالة append_str:
  تُضيف السلسلة src إلى نهاية dest وتُحدث حجم dest.
  (يمكن استخدام دالة مكتبية أو إعادة كتابتها كما هو موضح)
*/
char *append_str(char *dest, size_t *dest_size, const char *src)
{
    size_t src_len = ft_strlen(src);
    char *new_ptr = realloc(dest, *dest_size + src_len + 1);
    if (!new_ptr) {
        free(dest);
        perror("realloc");
        exit(EXIT_FAILURE);
    }
    ft_memcpy(new_ptr + *dest_size, src, src_len + 1);
    *dest_size += src_len;
    return new_ptr;
}

/*
  الدالة المعدلة:
  - تقوم بمعالجة رموز ">", ">>" و "<" كما في السابق.
  - عند العثور على here-document (<<) لا تقرأها على الفور وترسل مدخلها عبر dup2،
    بل تجمع محتويات جميع here-docs في متغير مؤقت (heredoc_all).
  - بعد الانتهاء من المرور على مصفوفة argv، يتم إنشاء أنبوب (pipe)
    وكتابة المحتوى المجمّع إليه، ثم إعادة توجيه STDIN إليه.
  - كما يتم إزالة رموز إعادة التوجيه ووسائطها من argv.
*/
void handle_redirections(t_execute *execute)
{
    char **argv = execute->commands[execute->i];
    int j = 0, k;
    char *heredoc_all = NULL;
    size_t heredoc_total_size = 0;

    while (argv[j])
    {
        if (ft_strcmp(argv[j], ">") == 0 ||
            ft_strcmp(argv[j], ">>") == 0 ||
            ft_strcmp(argv[j], "<") == 0)
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
            /* إزالة عامل إعادة التوجيه والوسيط من argv */
            k = j;
            while (argv[k+2])
            {
                argv[k] = argv[k+2];
                k++;
            }
            argv[k] = NULL;
            argv[k+1] = NULL;
            /* لا نقوم بزيادة j هنا لأن العنصر الجديد في الموضع j يجب فحصه */
        }
        else if (ft_strcmp(argv[j], "<<") == 0)
        {
            char *target = argv[j+1];
            if (!target)
            {
                ft_dprintf(STDERR_FILENO, "syntax error near unexpected token `%s'\n", argv[j]);
                exit(EXIT_FAILURE);
            }
            /* استخدام readline بدل getline */
            char *line;
            char *current_doc = NULL;
            size_t current_size = 0;

            while (1)
            {
                line = readline("> ");
                rl_on_new_line();  // تأكيد الانتقال لسطر جديد
                
                if (!line)
                {
                    ft_dprintf(STDERR_FILENO, "warning: here-document delimited by EOF (wanted `%s')\n", target);
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
       
            /* دمج current_doc في المحتوى الكلي heredoc_all */
            if (current_doc)
            {
                heredoc_all = append_str(heredoc_all, &heredoc_total_size, current_doc);
                free(current_doc);
            }
            /* إزالة رمز here-document والمحدد من argv */
            k = j;
            while (argv[k+2])
            {
                argv[k] = argv[k+2];
                k++;
            }
            argv[k] = NULL;
            argv[k+1] = NULL;
            /* لا نقوم بزيادة j لأن قد يكون هناك المزيد */
        }
        else
        {
            j++;
        }
    }

    /* إذا تم جمع محتوى here-document واحد أو أكثر، نقوم بإعادة توجيه STDIN إلى أنبوب يحتوي على المحتوى المدمج */
    if (heredoc_all)
    {
        int pipefd[2];
        if (pipe(pipefd) < 0)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        if (write(pipefd[1], heredoc_all, strlen(heredoc_all)) < 0)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
        free(heredoc_all);
        close(pipefd[1]);
        if (dup2(pipefd[0], STDIN_FILENO) < 0)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(pipefd[0]);
    }
}
