/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oissa <oissa@student.42amman.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 02:23:26 by lalhindi          #+#    #+#             */
/*   Updated: 2025/03/13 17:20:09 by oissa            ###   ########.fr       */
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
			ft_perror_free_exit("Erorr404: pipe", execute, main, redirections);
		if (write(pipefd[1], redirections->heredoc_all,
				redirections->heredoc_total_size) < 0)
			ft_perror_free_exit("Erorr404: write", execute, main, redirections);
		free(redirections->heredoc_all);
		close(pipefd[1]);
		if (dup2(pipefd[0], STDIN_FILENO) < 0)
			ft_perror_free_exit("Erorr404: dup2", execute, main, redirections);
		close(pipefd[0]);
	}
	free_redirections(redirections);
}

size_t	ft_determine_number_of_commands(t_execute *execute)
{
	size_t	num_command;
	size_t	i;
	size_t	n_commands;

	i = 0;
	n_commands = 0;
	num_command = 0;
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
	return (num_command);
}

void	handle_redirections(t_execute *execute, t_main *main)
{
	t_redirections	redirections;
	int				num_command;

	ft_bzero(&redirections, sizeof(t_redirections));
	redirections.exit_status = execute->exit_status;
	redirections.argv = execute->commands[execute->i];
	num_command = ft_determine_number_of_commands(execute);
	while (redirections.argv[redirections.j])
	{
		if (((!ft_strcmp(redirections.argv[redirections.j], ">")
					|| !ft_strcmp(redirections.argv[redirections.j], ">>")
					|| !ft_strcmp(redirections.argv[redirections.j], "<"))
				&& main->tokens_list[num_command
					+ redirections.j].type != TOKEN_ARGUMENT
				&& redirection_check(&redirections, main, execute) == 0))
			continue ;
		else if ((!ft_strcmp(redirections.argv[redirections.j], "<<")
				&& main->tokens_list[num_command
					+ redirections.j].type != TOKEN_ARGUMENT
				&& redirection_check_else_if(&redirections, execute,
					main) == 0))
			continue ;
		redirections.j++;
	}
	if_redirections_heredoc_all(&redirections, main, execute);
}
