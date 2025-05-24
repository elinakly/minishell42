/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:17:48 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/05/24 17:54:37 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Decides whether the command syntax is correct based on the last token type
 * @param last The last token type in a parsed command string
 * @returns A parsing result
 */
t_parse_result	get_parse_res_from_last(t_token_type last)
{
	if (last == TT_RE_IN || last == TT_RE_OUT || last == TT_RE_OUT_APPEND
		|| last == TT_HEREDOC)
		return (SYNTAX_ERROR);
	if (last == TT_PIPE)
		return (SYNTAX_ERROR);
	return (PARSEOK);
}

/**
 * Compares current token type to the previous token type to
 * decide whether the command syntax is correct up until the current token
 * @param curr The current token type
 * @param prev The previous token type
 * @returns A parsing result
 */
t_parse_result	comp_curr_last_types(t_token_type curr, t_token_type prev)
{
	bool	both_pipes;
	bool	subsequent_redirects;
	bool	either_it_redirect;
	bool	either_is_pipe;

	both_pipes = prev == TT_PIPE && curr == TT_PIPE;
	either_is_pipe = prev == TT_PIPE || curr == TT_PIPE;
	subsequent_redirects = ((prev == TT_RE_IN || prev == TT_RE_OUT
				|| prev == TT_RE_OUT_APPEND || prev == TT_HEREDOC)
			&& (curr == TT_RE_IN || curr == TT_RE_OUT
				|| curr == TT_RE_OUT_APPEND || curr == TT_HEREDOC));
	either_it_redirect = ((prev == TT_RE_IN || prev == TT_RE_OUT
				|| prev == TT_RE_OUT_APPEND || prev == TT_HEREDOC)
			|| (curr == TT_RE_IN || curr == TT_RE_OUT
				|| curr == TT_RE_OUT_APPEND || curr == TT_HEREDOC));
	if (both_pipes || subsequent_redirects
		|| (either_is_pipe && either_it_redirect))
		return (SYNTAX_ERROR);
	return (PARSEOK);
}

/**
 * Validates command syntax based on the order of token types
 * @param tokens The list of all parsed tokens
 * @returns A parsing result
 */
t_parse_result	validate_tokens(t_token *token)
{
	t_token_type	last;

	if (!token)
		return (EMPTY);
	last = token->type;
	token = token->next;
	if (last == TT_PIPE)
		return (SYNTAX_ERROR);
	while (token)
	{
		if (comp_curr_last_types(token->type, last) != PARSEOK)
			return (SYNTAX_ERROR);
		if (token->type == TT_HEREDOC_DELIM
			&& (!token->value || !ft_strlen(token->value)))
			return (SYNTAX_ERROR);
		last = token->type;
		token = token->next;
	}
	return (get_parse_res_from_last(last));
}

/**
 * Validates a command string before tokenizing takes place
 * Checks for unclosed quotes and an escape character at the end of the string
 * @param cmd The command string to validate
 * @returns A parsing result
 */
t_parse_result	validate_cmd_str(char *cmd)
{
	size_t			index;
	t_metachar		quotstate;

	index = 0;
	quotstate = MC_NONE;
	if (!cmd || !*cmd)
		return (EMPTY);
	index = skip_spaces(cmd);
	if (!cmd[index])
		return (EMPTY);
	while (cmd[index])
		set_quote_state(cmd, index++, &quotstate);
	if (cmd[index - 1] == '\\')
		return (SYNTAX_ERROR);
	if (quotstate == MC_SQUOTE)
		return (SYNTAX_ERROR);
	if (quotstate == MC_DQUOTE)
		return (SYNTAX_ERROR);
	return (PARSEOK);
}
