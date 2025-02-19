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

// int	smart_count_words(const char *str)
// {
// 	int		wc;
// 	t_split	s;

// 	wc = 0;
// 	ft_bzero(&s, sizeof(t_split));
// 	while (str[s.i])
// 	{
// 		update_quote_state(str[s.i], &s.sq, &s.dq);
// 		if (str[s.i] == ' ' && !s.sq && !s.dq)
// 		{
// 			if (s.i > s.start)
// 				wc++;
// 			s.start = s.i + 1;
// 		}
// 		s.i++;
// 	}
// 	if (s.i > s.start)
// 		wc++;
// 	return (wc);
// }

// char	**smart_split(const char *str)
// {
// 	char		**res;
// 	t_split		s;
// 	const char	*original;
// 	int			word_count;

// 	original = str;
// 	ft_bzero(&s, sizeof(t_split));
// 	word_count = smart_count_words(str);
// 	res = (char **)ft_calloc(word_count + 1, sizeof(char *));
// 	if (!res)
// 		return (NULL);
// 	while (str[s.i])
// 	{
// 		update_quote_state(str[s.i], &s.sq, &s.dq);
// 		if (str[s.i] == ' ' && !s.sq && !s.dq)
// 		{
// 			if (s.i > s.start)
// 				res[s.wc++] = ft_substr(original, s.start, s.i - s.start);
// 			s.start = s.i + 1;
// 		}
// 		s.i++;
// 	}
// 	if (s.i > s.start)
// 		res[s.wc++] = ft_substr(original, s.start, s.i - s.start);
// 	res[s.wc] = NULL;
// 	return (res);
// }

// static int	count_quoted_words(const char *str)
// {
//     t_split	s;

//     ft_bzero(&s, sizeof(s));
//     while (str[s.i])
//     {
//         update_quote_state(str[s.i], &s.sq, &s.dq);
//         if (str[s.i] == ' ' && !s.sq && !s.dq)
//         {
//             if (s.i > s.start)
//                 s.wc++;
//             s.start = s.i + 1;
//         }
//         s.i++;
//     }
//     if (s.i > s.start)
//         s.wc++;
//     return (s.wc);
// }

// char	**smart_split(const char *str)
// {
//     char		**res;
//     t_split		s;
//     const char	*original;
//     int			word_count;

//     original = str;
//     ft_bzero(&s, sizeof(t_split));
//     word_count = count_quoted_words(str);
// 	// word_count = count_args((char *)str);
//     res = (char **)ft_calloc(word_count + 1, sizeof(char *));
//     if (!res)
//         return (NULL);
//     while (str[s.i])
//     {
//         update_quote_state(str[s.i], &s.sq, &s.dq);
//         if (str[s.i] == ' ' && !s.sq && !s.dq)
//         {
//             if (s.i > s.start)
//                 res[s.wc++] = ft_substr(original, s.start, s.i - s.start);
//             s.start = s.i + 1;
//         }
//         s.i++;
//     }
//     if (s.i > s.start)
//         res[s.wc++] = ft_substr(original, s.start, s.i - s.start);
//     res[s.wc] = NULL;
//     return (res);
// }

// #include "minishell.h"
static int count_quoted_words(const char *str)
{
	t_split s;
	ft_bzero(&s, sizeof(s));
	while (str[s.i])
	{
		update_quote_state(str[s.i], &s.sq, &s.dq);
		if ((!s.sq && !s.dq) && (str[s.i] == ' ' || str[s.i] == '|' ||
								 str[s.i] == '<' || str[s.i] == '>'))
		{
			if (s.i > s.start)
				s.wc++;
			if (str[s.i] == '|' || str[s.i] == '<' || str[s.i] == '>')
				s.wc++;
			s.start = s.i + 1;
		}
		s.i++;
	}
	if (s.i > s.start)
		s.wc++;
	return (s.wc);
}

char **smart_split(const char *str)
{
	char **res;
	t_split s;
	const char *original;
	int word_count;

	original = str;
	ft_bzero(&s, sizeof(t_split));
	word_count = count_quoted_words(str);
	res = (char **)ft_calloc(word_count + 1, sizeof(char *));
	if (!res)
		return (NULL);
	while (str[s.i])
	{
		update_quote_state(str[s.i], &s.sq, &s.dq);
		if ((!s.sq && !s.dq) && (str[s.i] == ' ' || str[s.i] == '|' ||
								 str[s.i] == '<' || str[s.i] == '>'))
		{
			if (s.i > s.start)
				res[s.wc++] = ft_substr(original, s.start, s.i - s.start);
			if (str[s.i] == '|' || str[s.i] == '<' || str[s.i] == '>')
				res[s.wc++] = ft_substr(original, s.i, (str[s.i + 1] == str[s.i]) ? 2 : 1);
			s.start = s.i + ((str[s.i + 1] == str[s.i]) ? 2 : 1);
			if (str[s.i + 1] == str[s.i])
				s.i++;
		}
		s.i++;
	}
	if (s.i > s.start)
		res[s.wc++] = ft_substr(original, s.start, s.i - s.start);
	res[s.wc] = NULL;
	return (res);
}

int init_parse_cmd(t_parse_cmd *parse_cmd, char *input)
{
	ft_bzero(parse_cmd, sizeof(t_parse_cmd));
	ft_bzero(&parse_cmd->cmd, sizeof(t_cmd));
	parse_cmd->trimmed_input = ft_strtrim(input, " \t\n");
	parse_cmd->clean_input = ft_strdup(parse_cmd->trimmed_input);
	parse_cmd->splitter_clean_input = smart_split(parse_cmd->clean_input);
	for (int i = 0; parse_cmd->splitter_clean_input[i]; i++)
		ft_printf("%s splitter_clean_input\n", parse_cmd->splitter_clean_input[i]);
	free(parse_cmd->trimmed_input);
	if (!parse_cmd->clean_input)
		return (EXIT_FAILURE);
	parse_cmd->buffer = ft_calloc((ft_strlen(parse_cmd->clean_input) + 1), sizeof(char));
	if (!parse_cmd->buffer)
	{
		free(parse_cmd->clean_input);
		return (EXIT_FAILURE);
	}
	parse_cmd->cmd.arg_count = count_args(parse_cmd->clean_input);
	parse_cmd->cmd.args = ft_calloc((parse_cmd->cmd.arg_count + 1), sizeof(char *));
	if (!parse_cmd->cmd.args)
	{
		free(parse_cmd->clean_input);
		free(parse_cmd->buffer);
		return (EXIT_FAILURE);
	}
	parse_cmd->cmd.args[parse_cmd->cmd.arg_count] = NULL;
	return (EXIT_SUCCESS);
}

int if_token_started(t_parse_cmd *parse_cmd, t_env *env_list)
{
	if (parse_cmd->token_started)
	{
		parse_cmd->buffer[parse_cmd->j] = '\0';
		parse_cmd->index_splitter = parse_cmd->i;
		if ((parse_cmd->token_was_single_quoted || parse_cmd->token_was_dollar_quote) || !ft_strchr(parse_cmd->buffer, '$'))
		{
			parse_cmd->cmd.args[parse_cmd->i++] = ft_strdup(parse_cmd->buffer);
			if (parse_cmd->cmd.args[parse_cmd->i - 1] == NULL)
				return (EXIT_FAILURE);
		}
		else
		{

			parse_cmd->cmd.args[parse_cmd->i++] = expand_env_variables_in_token(parse_cmd->buffer,
																				env_list, parse_cmd);
		}
		parse_cmd->j = 0;
		parse_cmd->token_started = false;
		parse_cmd->token_quote_type = '\0';
		parse_cmd->token_was_single_quoted = false;
		parse_cmd->token_was_dollar_quote = false;

		
	}
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
			squote = !squote;
		else if (str[i] == '\"' && !squote)
			dquote = !dquote;
		else if (str[i] == '$' && !squote)
			return 1;
		i++;
	}
	return 0;
}

int if_token_started_three(t_parse_cmd *parse_cmd, t_env *env_list)
{
	if (parse_cmd->token_started)
	{
		parse_cmd->index_splitter = parse_cmd->i;
		parse_cmd->buffer[parse_cmd->j] = '\0';
		if ((parse_cmd->token_was_single_quoted || parse_cmd->token_was_dollar_quote) && !is_dollar_inside_quote(parse_cmd->clean_input))
		{
			parse_cmd->cmd.args[parse_cmd->i++] = ft_strdup(parse_cmd->buffer);
			if (parse_cmd->cmd.args[parse_cmd->i - 1] == NULL)
				return (EXIT_FAILURE);
		}
		else
		{
			parse_cmd->cmd.args[parse_cmd->i++] = expand_env_variables_in_token(parse_cmd->buffer,
																				env_list, parse_cmd);
		}
		// if (parse_cmd->splitter_clean_input[parse_cmd->index_splitter] != NULL)
	}
	return (EXIT_SUCCESS);
}

int parse_cmd_loop(t_parse_cmd *parse_cmd, t_env *env_list)
{
	while (parse_cmd->clean_input[parse_cmd->k])
	{
		parse_cmd->c = parse_cmd->clean_input[parse_cmd->k];
		if (ft_is_operator(parse_cmd->clean_input + parse_cmd->k) == 2)
			parse_cmd->operator= parse_cmd->clean_input[++parse_cmd->k];
		if (check_condition(parse_cmd) == EXIT_SUCCESS)
			continue;
		if (check_condition_too(parse_cmd, env_list) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		parse_cmd->operator= '\0';
	}
	return (EXIT_SUCCESS);
}

t_cmd *parse_cmd(char *input, t_env *env_list)
{
	t_parse_cmd parse_cmd;
	t_cmd *cmd_result;

	cmd_result = ft_calloc(1, sizeof(t_cmd));
	if ((!cmd_result || init_parse_cmd(&parse_cmd, input)) && !free_cmd_parse(&parse_cmd, cmd_result))
		return (NULL);
	if ((parse_cmd_loop(&parse_cmd, env_list) || if_token_started_three(&parse_cmd, env_list)) && !free_cmd_parse(&parse_cmd, cmd_result))
		return (NULL);
	if (parse_cmd.in_quotes && !free_cmd_parse(&parse_cmd, cmd_result))
		return (NULL);
	if (clean_parse_cmd(&parse_cmd) && !free_cmd_parse(&parse_cmd, cmd_result))
		return (NULL);

	*cmd_result = parse_cmd.cmd;
	return (cmd_result);
}
