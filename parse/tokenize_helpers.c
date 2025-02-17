/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize_helpers.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: mika <mika@student.42.fr>                    +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/17 13:55:07 by mika          #+#    #+#                 */
/*   Updated: 2025/02/17 20:14:07 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include "tokenize.h"

/**
 * Skips any spaces until finding another character or end of string
 * Does this by increasing the char pointer you point to
 * @param str The string you want to skip spaces on
 * @returns The number of spaces it skipped
 */
int	skip_spaces(char *str)
{
	int	spaces;

	spaces = 0;
	while (str && str[spaces] == ' ')
		spaces++;
	return (spaces);
}

/**
 * Checks whether a character is being escaped based on
 * the amount of backslashes that come before it
 * @param str The string to look inside
 * @param index The index at which the character is positioned
 * @returns `true` if character is escaped, `false` if not
 */
bool	is_escaped_char(char *str, size_t index)
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
int	skip_quoted(char *str)
{
	int		count;
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
			if (quote_type == '\'')
				break;
			if (quote_type == '"' && !is_escaped_char(str, count))
				break;
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
