/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mika <mika@student.42.fr>                    +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/17 13:46:21 by mika          #+#    #+#                 */
/*   Updated: 2025/02/25 16:20:38 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/**
 * This is all still experimental stage but gotta start somewhere!
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "memory/memory.h"
#include "tokenize.h"

int		skip_spaces(char *str);
int		skip_quoted(char *str);
bool	is_quote_char(char c, e_quote_type *type);
size_t	skip_meta(char *str);
bool	is_meta(char *str, size_t index, e_metachar *meta);
bool	disrupts_token(e_metachar meta);

/**
 * Counts how big a single token at a given position in a string is
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
	while (str[index] && str[index] != ' ')
	{
		if (!disrupts_token(meta) && is_meta(str, index, &meta))
			index += skip_meta(&str[index]);
		if (disrupts_token(meta) || !str[index] || str[index] == ' ')
			break;
		while (str[index] && str[index] != ' ' && !is_meta(str, index, &meta))
			index++;
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
 * TODO: Use libft version instead (I copied it from there since I cba to dump libft in here yet)
 */
size_t	ms_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	srcsize;
	size_t	ctr;

	srcsize = 0;
	ctr = 0;
	while (src[srcsize])
		srcsize++;
	if (size > 0)
	{
		while (*src && ctr < size - 1)
		{
			*(dst++) = *(src++);
			ctr++;
		}
		*dst = '\0';
	}
	return (srcsize);
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
	ms_strlcpy(result, str, size + 1);
	return (result);
}

e_token_type	get_token_type(char *raw_token, e_token_type last, bool *cmdfound)
{
	size_t	len;

	len = 0;
	while (raw_token[len]) //TODO: Probably just replace with ft_strlen xd
		len++;
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
	if (!*cmdfound)
		return (*cmdfound = true, TT_COMMAND);
	if (last == TT_ARGUMENT || last == TT_COMMAND || last == TT_INFILE || last == TT_OUTFILE || last == TT_HEREDOC)
		return (TT_ARGUMENT);
	return (TT_COMMAND);
}

t_token	*make_token(char *raw_token, e_token_type last)
{
	t_token	*token;

	if (!raw_token)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = raw_token;

}

/**
 * Takes a full minishell command and tokenizes it (splitting it up)
 * @param entry The string to tokenize
 * @returns An array of tokens (strings) or NULL if fail
 */
char	**tokenize(char *entry, size_t *tokencount)
{
	size_t	count;
	size_t	index;
	size_t	str_index;
	char	**result;

	count = count_tokens(entry);
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	result[count] = NULL;
	index = 0;
	str_index = 0;
	while (index < count)
	{
		result[index] = get_raw_token(&entry[str_index]);
		if (!result[index])
			return (free_array((void **)result), NULL);
		str_index += token_size(&entry[str_index], true);
		index++;
	}
	if (tokencount)
		*tokencount = count;
	return (result);
}


