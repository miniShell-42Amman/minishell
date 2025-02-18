/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalhindi <lalhindi@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 18:18:01 by oissa             #+#    #+#             */
/*   Updated: 2025/02/16 02:37:54 by lalhindi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	smart_count_words(const char *str)
{
	int		wc;
	t_split	s;

	ft_bzero(&s, sizeof(s));
	while (str[s.i])
	{
		update_quote_state(str[s.i], &s.sq, &s.dq);
		if (str[s.i] == ' ' && !s.sq && !s.dq)
		{
			if (s.i > s.start)
				wc++;
			s.start = s.i + 1;
		}
		s.i++;
	}
	if (s.i > s.start)
		wc++;
	return (wc);
}

char	**smart_split(const char *str)
{
	char		**res;
	t_split		s;
	const char	*original;
	int			word_count;

	original = str;
	ft_bzero(&s, sizeof(s));
	word_count = smart_count_words(str);
	res = ft_calloc(word_count + 1, sizeof(char *));
	if (!res)
		return (NULL);
	while (str[s.i])
	{
		update_quote_state(str[s.i], &s.sq, &s.dq);
		if (str[s.i] == ' ' && !s.sq && !s.dq)
		{
			if (s.i > s.start)
				res[s.wc++] = ft_substr(original, s.start, s.i - s.start);
			s.start = s.i + 1;
		}
		s.i++;
	}
	if (s.i > s.start)
		res[s.wc++] = ft_substr(original, s.start, s.i - s.start);
	res[s.wc] = NULL;
	return (res);
}

int	init_parse_cmd(t_parse_cmd *parse_cmd, char *input)
{
	ft_bzero(parse_cmd, sizeof(t_parse_cmd));
	ft_bzero(&parse_cmd->cmd, sizeof(t_cmd));
	parse_cmd->trimmed_input = ft_strtrim(input, " \t\n");
	parse_cmd->clean_input = ft_strdup(parse_cmd->trimmed_input);
	parse_cmd->splitter_clean_input = smart_split(parse_cmd->clean_input);
	free(parse_cmd->trimmed_input);
	if (!parse_cmd->clean_input)
		return (EXIT_FAILURE);
	parse_cmd->buffer = ft_calloc((ft_strlen(parse_cmd->clean_input) + 1),sizeof(char));
	if (!parse_cmd->buffer)
	{
		free(parse_cmd->clean_input);
		return (EXIT_FAILURE);
	}
	parse_cmd->cmd.arg_count = count_args(parse_cmd->clean_input);
	parse_cmd->cmd.args = ft_calloc((parse_cmd->cmd.arg_count + 1),
									sizeof(char *));
	if (!parse_cmd->cmd.args)
	{
		free(parse_cmd->clean_input);
		free(parse_cmd->buffer);
		return (EXIT_FAILURE);
	}
	parse_cmd->cmd.args[parse_cmd->cmd.arg_count] = NULL;
	return (EXIT_SUCCESS);
}

int	if_token_started(t_parse_cmd *parse_cmd, t_env *env_list)
{
	if (parse_cmd->token_started)
	{
		ft_printf("1\n");
		parse_cmd->buffer[parse_cmd->j] = '\0';
		if ((parse_cmd->token_was_single_quoted
			|| parse_cmd->token_was_dollar_quote) || !ft_strchr(parse_cmd->buffer, '$'))
			{

				ft_printf("2\n");
				parse_cmd->cmd.args[parse_cmd->i++] = ft_strdup(parse_cmd->buffer);
				if (parse_cmd->cmd.args[parse_cmd->i - 1] == NULL)
				return (EXIT_FAILURE);
			}
			else
			{	parse_cmd->cmd.args[parse_cmd->i++]
				= expand_env_variables_in_token(parse_cmd->buffer,
					env_list, parse_cmd);
					
					ft_printf("3\n");
				}
				parse_cmd->j = 0;
				parse_cmd->token_started = false;
				parse_cmd->token_quote_type = '\0';
				parse_cmd->token_was_single_quoted = false;
				parse_cmd->token_was_dollar_quote = false;
			}
			parse_cmd->index_splitter++;
			ft_printf("4\n");
			return (EXIT_SUCCESS);
}
int is_dollar_inside_quote(const char *str)
{
	int i;
	bool squote;
	bool dquote;

	i = 0;
	squote = false;
	dquote = false;
	while (str[i])
	{
		if (str[i] == '\'' && !dquote)
		{
			squote = !squote;
			ft_printf("if   single is %d double is %d\n", squote, dquote);
		}
		else if (str[i] == '\"' && !squote)
		{
			ft_printf("else if   single is %d double is %d\n", squote, dquote);
			dquote = !dquote;
		}
		else if (str[i] == '$' && !squote)
		{
			ft_printf("single is %d double is %d\n", squote, dquote);
			return 1;
		}
		i++;
	}
	return 0;
}
		
int	if_token_started_three(t_parse_cmd *parse_cmd, t_env *env_list)
		{
			if (parse_cmd->token_started)
			{
		ft_printf("5\n");
		parse_cmd->buffer[parse_cmd->j] = '\0';
		// ft_printf("parse_cmd->token_was_single_quoted %d parse_cmd->token_was_dollar_quote %d parse_cmd->has_dollar %d\n", parse_cmd->token_was_single_quoted, parse_cmd->token_was_dollar_quote, parse_cmd->has_dollar);
		if ((parse_cmd->token_was_single_quoted
			|| parse_cmd->token_was_dollar_quote) && !is_dollar_inside_quote(parse_cmd->clean_input))
			{
				ft_printf("6\n");
				parse_cmd->cmd.args[parse_cmd->i++] = ft_strdup(parse_cmd->buffer);
				if (parse_cmd->cmd.args[parse_cmd->i - 1] == NULL)
				return (EXIT_FAILURE);
			}
			else
			{
				ft_printf("7\n");
				parse_cmd->cmd.args[parse_cmd->i++]
				= expand_env_variables_in_token(parse_cmd->buffer,
					env_list, parse_cmd);
					
				}
			}
			ft_printf("8\n");
	parse_cmd->index_splitter++;
	return (EXIT_SUCCESS);
}

int	parse_cmd_loop(t_parse_cmd *parse_cmd, t_env *env_list)
{
	while (parse_cmd->clean_input[parse_cmd->k])
	{
		parse_cmd->c = parse_cmd->clean_input[parse_cmd->k];
		if (ft_is_operator(parse_cmd->clean_input + parse_cmd->k) == 2)
			parse_cmd->operator= parse_cmd->clean_input[++parse_cmd->k];
		if (check_condition(parse_cmd) == EXIT_SUCCESS)
			continue ;
		if (check_condition_too(parse_cmd, env_list) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		parse_cmd->operator= '\0';
	}
	return (EXIT_SUCCESS);
}

t_cmd	*parse_cmd(char *input, t_env *env_list)
{
	t_parse_cmd	parse_cmd;
	t_cmd		*cmd_result;

	cmd_result = ft_calloc(1, sizeof(t_cmd));
	if ((!cmd_result || init_parse_cmd(&parse_cmd, input))
		&& !free_cmd_parse(&parse_cmd, cmd_result))
		return (NULL);
	if ((parse_cmd_loop(&parse_cmd, env_list)
			|| if_token_started_three(&parse_cmd, env_list))
		&& !free_cmd_parse(&parse_cmd, cmd_result))
		return (NULL);
	if (parse_cmd.in_quotes && !free_cmd_parse(&parse_cmd, cmd_result))
		return (NULL);
	if (clean_parse_cmd(&parse_cmd) && !free_cmd_parse(&parse_cmd, cmd_result))
		return (NULL);
	*cmd_result = parse_cmd.cmd;
	return (cmd_result);
}
