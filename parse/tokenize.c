/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mika <mika@student.42.fr>                    +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/17 13:46:21 by mika          #+#    #+#                 */
/*   Updated: 2025/02/17 18:11:01 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/**
 * This is all still experimental stage but gotta start somewhere!
 */

#include <stdio.h>
#include <stdbool.h>
#include "tokenize.h"

int		skip_spaces(char *str);
int		skip_quoted(char *str);
bool	is_quote_char(char c, e_quote_type *type);

/**
 * Counts how many tokens are in a full minishell command
 * 
 * Accounts for quoted values, quoted + non-quoted, escaped double quotes, etc
 * but assumes quotes are closed properly
 * @param entry The string to count tokens in
 * @returns The amount of tokens
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

char	**tokenize(char *entry)
{
	size_t	count;
}
