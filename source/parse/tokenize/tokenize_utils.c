/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:02:04 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/24 16:44:45 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Counts how big a single token at a given position in a string is
 * TODO: Calling skip_spaces 3 times to prevent trailing
 * 		empty token is not efficient, look into improving
 * @param str The string in which the token must be found (can be offset)
 * @param include_spaces Whether to include skipped spaces in the size
 * @returns The size of the found token
 */
size_t	token_size(char *str, bool include_spaces)
{
	size_t		index;
	size_t		skip_count;
	t_metachar	meta;

	skip_count = skip_spaces(str);
	index = skip_count;
	meta = MC_NONE;
	while (str[index] && !is_wspace(str[index]))
	{
		if (!disrupts_token(meta) && is_meta(str, index, &meta))
			index += skip_meta(&str[index]);
		if (disrupts_token(meta) || !str[index] || is_wspace(str[index]))
			break ;
		while (str[index] && !is_meta(str, index, &meta))
			index++;
		if (!str[index + skip_spaces(str + index)])
		{
			skip_count += skip_spaces(str + index);
			index += skip_spaces(str + index);
		}
	}
	if (include_spaces)
		return (index);
	return (index - skip_count);
}

/**
 * Counts how many tokens are in a full minishell command
 * 
 * Accounts for quoted values, quoted + non-quoted, escaped double quotes, etc
 * but assumes quotes are closed properly
 * @param entry The string to count tokens in
 * @returns The amount of tokens
 */
size_t	count_tokens(char *entry)
{
	size_t	token_count;
	size_t	index;

	token_count = 0;
	index = 0;
	if (!entry)
		return (0);
	while (entry[index])
	{
		index += token_size(&entry[index], true);
		token_count++;
	}
	return (token_count);
}

/**
 * Checks whether a character is escapable in its current context
 * It differs between double quotes / single quotes / no quotes
 * @param c The character to check
 * @param quot The current quote state of the string of
 * 		which the character is a part
 * @returns `true` if character is escapable, `false` if not
 */
bool	can_escape(char c, t_metachar quot)
{
	if (quot == MC_NONE)
		return (c == MC_DQUOTE
			|| c == MC_SQUOTE
			|| c == MC_PIPE
			|| c == MC_ESCAPE
			|| c == MC_REDIR_IN
			|| c == MC_REDIR_OUT
			|| c == MC_VARIABLE);
	if (quot == MC_DQUOTE)
		return (c == MC_DQUOTE
			|| c == MC_ESCAPE
			|| c == MC_VARIABLE);
	else
		return (false);
}

/**
 * Calculates how many characters shorter a token will be after
 * removing quotes and escapes
 * @param token The token string for which the calculation should be done
 * @returns The number of characters the string length will decrease by
 */
size_t	calc_decrease(char *token)
{
	size_t		decrease;
	size_t		index;
	t_metachar	quot;

	decrease = 0;
	index = 0;
	quot = MC_NONE;
	while (token[index])
	{
		if (set_quote_state(token, index, &quot) && quot != MC_NONE)
			decrease += 2;
		else if (is_escaped_char(token, index)
			&& can_escape(token[index], quot) && quot == MC_DQUOTE)
			decrease++;
		else if (is_escaped_char(token, index) && quot == MC_NONE)
			decrease++;
		index++;
	}
	return (decrease);
}

t_token_type	get_token_type2(t_token_type last, bool *cmdfound)
{
	if (last == TT_RE_OUT || last == TT_RE_OUT_APPEND)
		return (TT_OUTFILE);
	if (last == TT_RE_IN)
		return (TT_INFILE);
	if (last == TT_HEREDOC)
		return (TT_HEREDOC_DELIM);
	if (!*cmdfound)
		return (*cmdfound = true, TT_COMMAND);
	if (last == TT_ARGUMENT || last == TT_COMMAND || last == TT_INFILE
		|| last == TT_OUTFILE || last == TT_HEREDOC_DELIM)
		return (TT_ARGUMENT);
	return (TT_COMMAND);
}
