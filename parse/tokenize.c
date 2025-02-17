/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenize.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mika <mika@student.42.fr>                    +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/17 13:46:21 by mika          #+#    #+#                 */
/*   Updated: 2025/02/17 14:51:54 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/**
 * This is all still experimental stage but gotta start somewhere!
 */

#include <stdio.h>

int	skip_spaces(char **str);

size_t	count_tokens(char *entry)
{
	int	in_dquote;
	int	in_squote;
	int	token_count;

	in_dquote = 0;
	in_squote = 0;
	token_count = 0;

	skip_spaces(&entry);
	
}