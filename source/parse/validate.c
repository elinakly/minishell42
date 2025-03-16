/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   validate.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: Mika Schipper <mschippe@student.codam.n      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/16 16:17:48 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/03/16 17:41:51 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/tokenandvar.h"
#include "../../include/structbuild.h"
#include "../../include/tokenize.h"

e_parse_result get_parse_res_from_last(e_token_type last)
{
	if (last == TT_RE_IN || last == TT_RE_OUT || last == TT_RE_OUT_APPEND)
		return (SYNTAX_ERROR);
	if (last == TT_PIPE)
		return (UNFINISHED_PIPE);
	return (PARSEOK);
}

e_parse_result comp_curr_last_types(e_token_type curr, e_token_type last)
{
	if ((last == TT_PIPE && curr == TT_PIPE)
			|| ((last == TT_RE_IN || last == TT_RE_OUT
				|| last == TT_RE_OUT_APPEND || last == TT_HEREDOC)
				&& (curr == TT_RE_IN
					|| curr == TT_RE_OUT
					|| curr == TT_RE_OUT_APPEND
					|| curr == TT_HEREDOC)))
		return (SYNTAX_ERROR);
	return (PARSEOK);
}

e_parse_result	validate_tokens(t_token **tokens) //TODO: Validate heredoc tokens
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
		last = tokens[index]->type;
		index++;
	}
	return (get_parse_res_from_last(last));
}

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
	if (cmd[index - 1] == '\\') //NOTE: Must come before the others, since it takes priority
		return (ESCAPED_NEWLINE);
	if (quotstate == MC_SQUOTE)
		return (UNCLOSED_SQUOTE);
	if (quotstate == MC_DQUOTE)
		return (UNCLOSED_DQUOTE);
	return (PARSEOK);
}
