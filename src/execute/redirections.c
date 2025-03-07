#include "minishell.h"
#include "string.h"
#include <errno.h>

char *append_str(char *dest, size_t *dest_size, const char *src)
{
	size_t src_len;
	char *new_ptr;

	src_len = ft_strlen(src);
	new_ptr = ft_realloc(dest, *dest_size, *dest_size + src_len + 1);
	if (!new_ptr)
	{
		free(dest);
	}
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

void choose_flags_fd(t_redirections *redirections, int *flags, int *fd,
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
		ft_dprintf(STDERR_FILENO, "Erorr404: %s: %s\n", redirections->target, strerror(errno));
		*redirections->exit_status = 1;
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
	if (!redirections->target && ft_dprintf(STDERR_FILENO,
											"Erorr404: syntax error near token `%s'\n", redirections->op))
	{
		free_redirections(redirections);
		exit(EXIT_FAILURE);
	}
	choose_flags_fd(redirections, &flags, &fd, &std_fd);
	if (dup2(fd, std_fd) < 0)
	{
		perror("Erorr404: dup2");
		free_redirections(redirections);
		exit(EXIT_FAILURE);
	}
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

void redirection_check_else_if_loop(t_redirections *redirections,
									t_here_document *here_doc, t_execute *execute)
{
	(void)execute;
	while (1)
	{
		here_doc->line = readline("> ");
		if (g_signal == 130)
		{
			free_here_doc(here_doc);
			free_redirections(redirections);
			// free_execute(execute, 0);
			break;
		}
		if (!here_doc->line)
		{
			ft_dprintf(STDERR_FILENO,
					   "Erorr404: warning: here-document delimited by EOF\n");
			free_here_doc(here_doc);
			break;
		}
		if (ft_strcmp(here_doc->line, here_doc->target) == 0)
		{
			free_here_doc(here_doc);
			break;
		}
		here_doc->current_doc = append_str(here_doc->current_doc,
										   &here_doc->current_size, here_doc->line);
		here_doc->current_doc = append_str(here_doc->current_doc,
										   &here_doc->current_size, "\n");
		free(here_doc->line);
	}
	if (here_doc->current_doc)
	{
		redirections->heredoc_all = append_str(redirections->heredoc_all,
											   &redirections->heredoc_total_size, here_doc->current_doc);
		free_here_doc(here_doc);
	}
}

int redirection_check_else_if(t_redirections *redirections, t_execute *execute)
{
	t_here_document here_doc;

	ft_bzero(&here_doc, sizeof(t_here_document));
	here_doc.target = redirections->argv[redirections->j + 1];
	if (!here_doc.target)
	{
		ft_dprintf(STDERR_FILENO, "Erorr404: syntax error near token `<<'\n");
		free_redirections(redirections);
		free_here_doc(&here_doc);
		*redirections->exit_status = 2;
		redirections = NULL;
		return (EXIT_FAILURE);
	}
	signal(SIGINT, handle_heredoc_sigint);
	redirection_check_else_if_loop(redirections, &here_doc, execute);
	if (redirections->argv && redirections->argv[redirections->j])
	{
			free(redirections->argv[redirections->j]);
			redirections->argv[redirections->j] = NULL;
	}
	redirections->k = redirections->j;
	while (redirections->argv[redirections->k + 2])
	{
		redirections->argv[redirections->k] = redirections->argv[redirections->k + 2];
		redirections->k++;
	}
	redirections->argv[redirections->k] = NULL;
	free_here_doc(&here_doc);
	return (EXIT_SUCCESS);
}
void if_redirections_heredoc_all(t_redirections *redirections)
{
	int pipefd[2];

	if (redirections->heredoc_all)
	{
		if (pipe(pipefd) < 0)
		{
			perror("Erorr404: pipe");
			free_redirections(redirections);
			exit(EXIT_FAILURE);
		}
		if (write(pipefd[1], redirections->heredoc_all,
				  redirections->heredoc_total_size) < 0)
		{
			perror("Erorr404: write");
			free_redirections(redirections);
			exit(EXIT_FAILURE);
		}
		free(redirections->heredoc_all);
		close(pipefd[1]);
		if (dup2(pipefd[0], STDIN_FILENO) < 0)
		{
			perror("Erorr404: dup2");
			free_redirections(redirections);
			exit(EXIT_FAILURE);
		}
		close(pipefd[0]);
	}
	free_redirections(redirections);
}

size_t ft_determine_number_of_commands(t_execute *execute)
{
	size_t num_command = 0;
	size_t i = 0;
	size_t n_commands = 0;
	while (i < execute->i)
	{
		while (execute->commands[i][n_commands])
		{
			n_commands++;
		}
		if (execute->i > 0)
			num_command++;
		num_command += n_commands;
		n_commands = 0;
		i++;
	}
	return num_command;
}

void handle_redirections(t_execute *execute, t_token *tokens)
{
	t_redirections redirections;
	int num_command;
	ft_bzero(&redirections, sizeof(t_redirections));
	redirections.exit_status = execute->exit_status;
	redirections.argv = execute->commands[execute->i];
	num_command = ft_determine_number_of_commands(execute);
	while (redirections.argv[redirections.j])
	{
		if (((!ft_strcmp(redirections.argv[redirections.j], ">")) || (!ft_strcmp(redirections.argv[redirections.j], ">>")) || (!ft_strcmp(redirections.argv[redirections.j], "<"))) && tokens[num_command + redirections.j].type != TOKEN_ARGUMENT && !redirection_check(&redirections))
			continue;
		else if ((!ft_strcmp(redirections.argv[redirections.j], "<<")) && tokens[num_command + redirections.j].type != TOKEN_ARGUMENT && !redirection_check_else_if(&redirections, execute))
			continue;
		redirections.j++;
	}
	if_redirections_heredoc_all(&redirections);
}
