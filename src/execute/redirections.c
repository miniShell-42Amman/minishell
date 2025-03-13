/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 02:23:26 by lalhindi          #+#    #+#             */
/*   Updated: 2025/03/14 00:53:32 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	}
	if (!new_ptr)
	{
		free(dest);
		perror("Erorr404: realloc");
		return (NULL);
	}
	ft_memcpy(new_ptr + *dest_size, src, src_len + 1);
	*dest_size += src_len;
	new_ptr[*dest_size] = '\0';
	return (new_ptr);
}

void	choose_flags_fd(t_redirections *redirections, t_fd_flags *fd_flags,
		t_main *main, t_execute *execute)
{
	fd_flags->std_fd = 0;
	fd_flags->flags = 0;
	if (ft_strcmp(redirections->op, ">") == 0)
		fd_flags->flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (ft_strcmp(redirections->op, ">>") == 0)
		fd_flags->flags = O_WRONLY | O_CREAT | O_APPEND;
	else if (ft_strcmp(redirections->op, "<") == 0)
		fd_flags->flags = O_RDONLY;
	fd_flags->fd = open(redirections->target, fd_flags->flags, 0644);
	if (fd_flags->fd < 0)
	{
		ft_dprintf(STDERR_FILENO, "Erorr404: %s: %s\n", redirections->target,
			strerror(errno));
		*redirections->exit_status = 1;
		free_execute(execute, 0);
		free_resources(main, 1);
		exit(EXIT_FAILURE);
	}
	if (fd_flags->flags == O_RDONLY)
		fd_flags->std_fd = STDIN_FILENO;
	else
		fd_flags->std_fd = STDOUT_FILENO;
}

void	if_redirections_heredoc_all(t_redirections *redirections, t_main *main,
		t_execute *execute)
{
	int	pipefd[2];

	if (redirections->heredoc_all)
	{
		if (pipe(pipefd) < 0)
			ft_perror_free_exit("pipe error", execute, main, redirections);
		if (write(pipefd[1], redirections->heredoc_all,
				redirections->heredoc_total_size) < 0)
			ft_perror_free_exit("write error", execute, main, redirections);
		close(pipefd[1]);
		if (dup2(pipefd[0], STDIN_FILENO) < 0)
			ft_perror_free_exit("dup2 error", execute, main, redirections);
		close(pipefd[0]);
	}
	free_redirections(redirections);
}

void	process_redirection(t_redirections *r, t_main *main, t_execute *ex,
		int i)
{
	t_fd_flags	fd_flags;

	if (!r->argv[i + 1])
	{
		ft_dprintf(2, "Syntax error near unexpected token `newline'\n");
		*ex->exit_status = 258;
		return ;
	}
	r->op = r->argv[i];
	r->target = r->argv[i + 1];
	choose_flags_fd(r, &fd_flags, main, ex);
	if (fd_flags.fd != -1)
	{
		if (dup2(fd_flags.fd, fd_flags.std_fd) == -1)
			ft_perror_free_exit("dup2 error", ex, main, r);
		close(fd_flags.fd);
	}
}

void	handle_redirections(t_execute *execute, t_main *main)
{
	t_redirections	redirections;
	int				num_command;
	int				i;

	i = 0;
	ft_bzero(&redirections, sizeof(t_redirections));
	redirections.exit_status = execute->exit_status;
	redirections.argv = execute->commands[execute->i];
	num_command = ft_determine_number_of_commands(execute);
	while (redirections.argv[i])
	{
		if (is_redirection(redirections.argv[i])
			&& main->tokens_list[num_command + i].type != TOKEN_ARGUMENT)
		{
			if (check_target_redirecion(&redirections, execute,
					i) == EXIT_BREAK)
				break ;
			process_redirection(&redirections, main, execute, i);
			i += 2;
		}
		else
			i++;
	}
	clean_arguments(execute, &redirections);
	if_redirections_heredoc_all(&redirections, main, execute);
}
