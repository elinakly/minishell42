/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize_helpers.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: mika <mika@student.42.fr>                    +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/17 13:55:07 by mika          #+#    #+#                 */
/*   Updated: 2025/02/17 15:41:55 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include "tokenize.h"

int	skip_spaces(char **str)
{
	int	spaces;

	spaces = 0;
	while (*str && (*str)[spaces] == ' ')
		spaces++;
	(*str) += spaces;
	return (spaces);
}

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

bool	openquotes(char *str, e_quote_type *type)
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


