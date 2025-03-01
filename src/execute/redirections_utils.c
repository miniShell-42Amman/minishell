
#include "minishell.h"

char	*append_str(char *dest, size_t *dest_size, const char *src)
{
	size_t	src_len;
	char	*new_ptr;

	src_len = ft_strlen(src);
	new_ptr = ft_realloc(dest, *dest_size, *dest_size + src_len + 1);
	if (!new_ptr)
	{
		free(dest);
		perror("Erorr404: realloc");
		exit(EXIT_FAILURE);
	}
	ft_memcpy(new_ptr + *dest_size, src, src_len + 1);
	*dest_size += src_len;
	new_ptr[*dest_size] = '\0';
	return (new_ptr);
}

void	choose_flags_fd(t_redirections *redirections, int *flags, int *fd,
		int *std_fd)
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
		perror("Erorr404: ");
		exit(EXIT_FAILURE);
	}
	if (*flags == O_RDONLY)
		*std_fd = STDIN_FILENO;
	else
		*std_fd = STDOUT_FILENO;
}

int	redirection_check(t_redirections *redirections)
{
	int	fd;
	int	flags;
	int	std_fd;

	redirections->op = redirections->argv[redirections->j];
	redirections->target = redirections->argv[redirections->j + 1];
	if (!redirections->target && ft_dprintf(STDERR_FILENO,
			"Erorr404: syntax error near token `%s'\n", redirections->op))
		exit(EXIT_FAILURE);
	choose_flags_fd(redirections, &flags, &fd, &std_fd);
	if (dup2(fd, std_fd) < 0)
		exit(EXIT_FAILURE);
	close(fd);
	redirections->k = redirections->j;
	while (redirections->argv[redirections->k + 2])
	{
		redirections->argv[redirections->k] = redirections->argv[redirections->k
			+ 2];
		redirections->k++;
	}
	redirections->argv[redirections->k] = NULL;
	return (EXIT_SUCCESS);
}