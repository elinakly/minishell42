/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:46:21 by mika              #+#    #+#             */
/*   Updated: 2025/03/19 11:10:42 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * This is all still experimental stage but gotta start somewhere!
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../include/variable.h"
#include "../../include/memory.h"
#include "../../include/tokenize.h"
#include "../../lib/libft/libft.h"

int		skip_spaces(char *str);
int		skip_quoted(char *str);
bool	is_quote_char(char c, e_quote_type *type);
size_t	skip_meta(char *str);
bool	is_meta(char *str, size_t index, e_metachar *meta);
bool	disrupts_token(e_metachar meta);
bool	set_quote_state(char *cmd, size_t index, e_metachar *current);
bool	is_escaped_char(char *str, int index);

/**
 * Counts how big a single token at a given position in a string is
 * TODO: Calling skip_spaces 3 times to prevent trailing empty token is not efficient, look into improving
 * @param str The string in which the token must be found (can be offset)
 * @param include_spaces Whether to include skipped spaces in the size
 * @returns The size of the found token
 */
size_t	token_size(char *str, bool include_spaces)
{
	size_t		index;
	size_t		skip_count;
	e_metachar	meta;

	skip_count = skip_spaces(str);
	index = skip_count;
	meta = MC_NONE;
	while (str[index] && !is_wspace(str[index]))
	{
		if (!disrupts_token(meta) && is_meta(str, index, &meta))
			index += skip_meta(&str[index]);
		if (disrupts_token(meta) || !str[index] || is_wspace(str[index]))
			break;
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
 * Creates a string for a single token at a given position in a string
 * @param str The string from which you want a token (can be offset)
 * @returns A malloc'd string with the token that was found or NULL on fail
 */
char	*get_raw_token(char *str)
{
	size_t	size;
	char	*result;

	str += skip_spaces(str);
	size = token_size(str, false);
	result = malloc(sizeof(char) * (size + 1));
	if (!result)
		return (NULL);
	ft_strlcpy(result, str, size + 1);
	return (result);
}

/**
 * Decides what token type a token is based on contents of the token
 * as well as contexts such as whether a command is already found in
 * the full string, and what the previous token type was
 * TODO: Break it up for 25 line norm
 * @param raw_token The token string
 * @param last The previous token type, used for context
 * @param cmdfound Whether a command token was found in the string already
 * @returns A token type enum
 */
e_token_type	get_token_type(char *raw_token, e_token_type last, bool *cmdfound)
{
	size_t	len;

	len = ft_strlen(raw_token);
	if (raw_token[0] == '|')
		return (*cmdfound = false, TT_PIPE);
	if (raw_token[0] == '>')
	{
		if (len < 2 || raw_token[1] != '>')
			return (TT_RE_OUT);
		else if (len > 1 && raw_token[1] == '>')
			return (TT_RE_OUT_APPEND);
	}
	if (raw_token[0] == '<')
	{
		if (len < 2 || raw_token[1] != '<')
			return (TT_RE_IN);
		else if (len > 1 && raw_token[1] == '<')
			return (TT_HEREDOC);
	}
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

/**
 * Takes a full minishell command and tokenizes it (splitting it up)
 * TODO: Not sure if it is here, but a single space appears to be a valid command when it should not be
 * @param entry The string to tokenize
 * @returns An array of tokens (strings) or NULL if fail
 */
char	**tokenize(char *entry, t_env_var **vars, size_t *amount)
{
	size_t	count;
	size_t	index;
	size_t	str_index;
	char	**result;

	entry = get_expanded_cmd(entry, vars);
	if (!entry)
		return (NULL);
	count = count_tokens(entry);
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (free(entry), NULL);
	result[count] = NULL;
	index = 0;
	str_index = 0;
	while (index < count)
	{
		result[index] = get_raw_token(&entry[str_index]);
		if (!result[index])
			return (free(entry), free_array((void **)result, NULL), NULL);
		str_index += token_size(&entry[str_index], true);
		index++;
	}
	*amount = count;
	return (free(entry), result);
}

/**
 * Checks whether a character is escapable in its current context
 * It differs between double quotes / single quotes / no quotes
 * @param c The character to check
 * @param quot The current quote state of the string of which the character is a part
 * @returns `true` if character is escapable, `false` if not
 */
bool	can_escape(char c, e_metachar quot)
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
	e_metachar	quot;

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

/**
 * Decides whether a character should sanitized or not based on 
 * quote states and various escape checks
 * TODO: Currently leaves in the extra escapes that we add into environment variables, fix by just not inserting those escapes maybe?
 * TODO: The above todo may actually be a HUGE problem with how I'm expanding vars, may need to rewrite that somehow
 * TODO: Just research the above todo's to get a clear answer on how to proceed
 * @param token The string of which the character is a part
 * @param index The index at which the character is inside `token`
 * @param quot The quote state metachar, used to track whether we are currently inside quotes
 * @returns `false` if character should be sanitized, `true` if not
 */
bool	incl_in_token(char *token, size_t index, e_metachar *quot)
{
	bool	quote_changed;
	bool	char_is_escaped;
	bool	can_esc;
	bool	next_can_esc;

	quote_changed = set_quote_state(token, index, quot);
	char_is_escaped = is_escaped_char(token, index);
	can_esc = can_escape(token[index], *quot);
	next_can_esc = token[index] != '\0' && can_escape(token[index + 1], *quot);
	if (quote_changed)
		return (false);
	if (*quot == MC_DQUOTE)
		return ((char_is_escaped)
			|| (!char_is_escaped && (token[index] != '\\' || !next_can_esc)));
	else if (*quot == MC_NONE)
		return ((token[index] != '\\') || char_is_escaped);
	return (true);
}

/**
 * Removes surrounding quotes and backslashes for valid escapes
 * Allocates memory for the new (smaller) token if necessary
 * DOES NOT free the previous, unsanitized token if any changes are made
 * TODO: Make sure not checking for token NULL is ok, but should be
 */
char	*sanitize_token(char *token)
{
	size_t		index;
	size_t		resindex;
	e_metachar	quot;
	e_metachar	meta;
	char		*res;

	index = 0;
	resindex = 0;
	quot = MC_NONE;
	meta = MC_NONE;
	res = malloc(sizeof(char) * (ft_strlen(token) - calc_decrease(token) + 1));
	if (!res)
		return (NULL);
	while (token[index])
	{
		if (incl_in_token(token, index, &quot))
			res[resindex++] = token[index];
		index++;
	}
	res[resindex] = '\0';
	return (res);
}
