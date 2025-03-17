/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   validate.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: Mika Schipper <mschippe@student.codam.n      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/16 16:17:48 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/03/17 11:44:53 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenandvar.h"
#include "../../include/structbuild.h"
#include "../../include/tokenize.h"
#include "../../lib/libft/libft.h"

/**
 * Decides whether the command syntax is correct based on the last token type
 * @param last The last token type in a parsed command string
 * @returns A parsing result
 */
e_parse_result get_parse_res_from_last(e_token_type last)
{
	if (last == TT_RE_IN || last == TT_RE_OUT || last == TT_RE_OUT_APPEND
		|| last == TT_HEREDOC)
		return (SYNTAX_ERROR);
	if (last == TT_PIPE)
		return (UNFINISHED_PIPE);
	return (PARSEOK);
}

/**
 * Compares current token type to the previous token type to
 * decide whether the command syntax is correct up until the current token
 * @param curr The current token type
 * @param prev The previous token type
 * @returns A parsing result
 */
e_parse_result comp_curr_last_types(e_token_type curr, e_token_type prev)
{
	if ((prev == TT_PIPE && curr == TT_PIPE)
			|| ((prev == TT_RE_IN || prev == TT_RE_OUT
				|| prev == TT_RE_OUT_APPEND || prev == TT_HEREDOC)
				&& (curr == TT_RE_IN
					|| curr == TT_RE_OUT
					|| curr == TT_RE_OUT_APPEND
					|| curr == TT_HEREDOC)))
		return (SYNTAX_ERROR);
	return (PARSEOK);
}

/**
 * Validates command syntax based on the order of token types
 * @param tokens The list of all parsed tokens
 * @returns A parsing result
 */
e_parse_result	validate_tokens(t_token **tokens)
{
	e_token_type	last;
	size_t			index;

	last = TT_UNKNOWN;
	index = 0;
	if (!tokens || !tokens[0])
		return (EMPTY);
	while (tokens[index])
	{
		if (index == 0)
		{
			last = tokens[index]->type;
			index++;
			continue;
		}
		else if (comp_curr_last_types(tokens[index]->type, last) != PARSEOK)
			return (SYNTAX_ERROR);
		if (tokens[index]->type == TT_HEREDOC_DELIM
			&& (!tokens[index]->value || !ft_strlen(tokens[index]->value)))
			return (SYNTAX_ERROR);
		last = tokens[index]->type;
		index++;
	}
	return (get_parse_res_from_last(last));
}

/**
 * Validates a command string before tokenizing takes place
 * Checks for unclosed quotes and an escape character at the end of the string
 * @param cmd The command string to validate
 * @returns A parsing result
 */
e_parse_result	validate_cmd_str(char *cmd)
{
	size_t			index;
	e_metachar		quotstate;

	index = 0;
	quotstate = MC_NONE;
	if (!cmd || !*cmd)
		return (EMPTY);
	while (cmd[index])
		set_quote_state(cmd, index++, &quotstate);
	if (cmd[index - 1] == '\\')
		return (ESCAPED_NEWLINE);
	if (quotstate == MC_SQUOTE)
		return (UNCLOSED_SQUOTE);
	if (quotstate == MC_DQUOTE)
		return (UNCLOSED_DQUOTE);
	return (PARSEOK);
}
