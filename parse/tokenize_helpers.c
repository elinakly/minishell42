/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize_helpers.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: mika <mika@student.42.fr>                    +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/17 13:55:07 by mika          #+#    #+#                 */
/*   Updated: 2025/02/17 16:00:53 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include "tokenize.h"

/**
 * Skips any spaces until finding another character or end of string
 * Does this by increasing the char pointer you point to
 * @param str The pointer to the char pointer you want to skip spaces on
 * @returns The number of spaces it skipped
 */
int	skip_spaces(char **str)
{
	int	spaces;

	spaces = 0;
	while (*str && (*str)[spaces] == ' ')
		spaces++;
	(*str) += spaces;
	return (spaces);
}

/**
 * Checks whether a double quote is being escaped
 * @param str The string to look inside
 * @param index The index at which the double quote character is positioned
 * @returns `true` if double quote is escaped, `false` if not
 */
bool	is_esc_dquote(char *str, int index)
{
	int	backslashes;

	if (str[index] != '"')
		return (false);
	backslashes = 0;
	index--;
	while (index > -1 && str[index] == '\\')
	{
		backslashes++;
		index--;
	}
	return (backslashes % 2);
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
		if (str[index] == '"' && !in_squotes && !is_esc_dquote(str, index))
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


