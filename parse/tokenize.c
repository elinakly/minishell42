/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mika <mika@student.42.fr>                    +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/17 13:46:21 by mika          #+#    #+#                 */
/*   Updated: 2025/02/17 17:46:33 by Mika Schipp   ########   odam.nl         */
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
		index += skip_spaces(&entry[index]);
		if (is_quote_char(entry[index], NULL))
			index += skip_quoted(&entry[index]);
		while (entry[index] && entry[index] != ' ')
			index++;
		token_count++;
		index++;
	}
	return (token_count);
}
