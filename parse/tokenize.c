/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mika <mika@student.42.fr>                    +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/17 13:46:21 by mika          #+#    #+#                 */
/*   Updated: 2025/02/17 18:48:13 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/**
 * This is all still experimental stage but gotta start somewhere!
 */

#include <stdbool.h>
#include <stdlib.h>
#include "tokenize.h"

int		skip_spaces(char *str);
int		skip_quoted(char *str);
bool	is_quote_char(char c, e_quote_type *type);

/**
 * Counts how big a single token at a given position in a string is
 * @param str The string in which the token must be found (can be offset)
 * @param include_spaces Whether to include skipped spaces in the size
 * @returns The size of the found token
 */
size_t	token_size(char *str, bool include_spaces)
{
	size_t	index;
	size_t	size;
	size_t	temp;

	index = 0;
	size = 0;
	index += skip_spaces(str);
	if (is_quote_char(str[index], NULL))
	{
		temp = skip_quoted(&str[index]);
		index += temp;
		size += temp;
	}
	while (str[index] && str[index] != ' ')
	{
		index++;
		size++;
	}
	if (include_spaces)
		return (index);
	return (size);
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
char	*get_token(char *str)
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

/**
 * Takes a full minishell command and tokenizes it (splitting it up)
 * @param entry The string to tokenize
 * @returns An array of tokens (strings) or NULL if fail
 */
char	**tokenize(char *entry)
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
		result[index] = get_token(&entry[str_index]);
		if (!result[index])
			return (NULL); /* TODO: Free array retroactively on fail */
		str_index += token_size(&entry[str_index], true);
		index++;
	}
	return (result);
}
