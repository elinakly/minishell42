/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:55:07 by mika              #+#    #+#             */
/*   Updated: 2025/05/20 16:01:56 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include "tokenize.h"

/**
 * Checks whether a character is being escaped based on
 * the amount of backslashes that come before it
 * @param str The string to look inside
 * @param index The index at which the character is positioned
 * @returns `true` if character is escaped, `false` if not
 */
bool	is_escaped_char(char *str, int index)
{
	size_t	backslashes;

	if (!str)
		return (false);
	backslashes = 0;
	index--;
	while (index > -1 && str[index] == '\\')
	{
		backslashes++;
		index--;
	}
	return (backslashes % 2) != 0;
}

/**
 * Skips from the current quote character all the way until after the closing
 * quote character
 * 
 * Assumes all quotes are properly closed
 * @param str The string on which you want to skip quoted
 * content
 * @returns The amount of characters that were skipped
 */
size_t	skip_quoted(char *str)
{
	size_t	count;
	char	quote_type;

	count = 0;
	if (str && (str[count] == '"' || str[count] == '\''))
		quote_type = str[count];
	else
		return (0);
	count++;
	while (str[count])
	{
		if (str[count] == quote_type)
		{
			break ;
			if (quote_type == '"' && !is_escaped_char(str, count))
				break ;
		}
		count++;
	}
	if (str[count] == quote_type)
		count++;
	return (count);
}

/**
 * Checks if a string has an unclosed double or single quote in it
 * Can also set a value at an enum pointer to indicate which quote is unclosed
 * @param str The string in which quotes should be checked
 * @param type A pointer to a quote type enum which will be set (can be NULL)
 * @returns `true` if any type of quote is unclosed, `false` if not
 */
bool	has_unclosed_quote(char *str, e_quote_type *type)
{
	bool	in_dquotes;
	bool	in_squotes;
	int		index;

	index = 0;
	in_dquotes = false;
	in_squotes = false;
	while (str && str[index])
	{
		if (str[index] == '"' && !in_squotes && !is_escaped_char(str, index))
			in_dquotes = !in_dquotes;
		if (str[index] == '\'' && !in_dquotes)
			in_squotes = !in_squotes;
		index++;
	}
	if (in_dquotes && type)
		*type = QUOTE_DOUBLE;
	else if (in_squotes && type)
		*type = QUOTE_SINGLE;
	else if (type)
		*type = QUOTE_NONE;
	return (in_dquotes || in_squotes);
}

/**
 * Checks whether a character is a quote (single or double) or not
 * Can also set a value at an enum pointer to indicate which quote
 * @param c The character to check
 * @param type A pointer to a quote type enum which will be set (can be NULL)
 * @returns `true` if the character is a quote, `false` if not
 */
bool	is_quote_char(char c, e_quote_type *type)
{
	if (type)
	{
		if (c == '\'')
			*type = QUOTE_SINGLE;
		else if (c == '"')
			*type = QUOTE_DOUBLE;
		else
			*type = QUOTE_NONE;
	}
	return (c == '\'' || c == '"');
}

/**
 * Simple helper function to determine if a character is breaking whitespace
 * @param c The character to check
 * @returns `true` if space/newline/tab, `false` if not
 */
bool	is_wspace(char c)
{
	return (c == '\n' || c == '\t' || c == ' ');
}
