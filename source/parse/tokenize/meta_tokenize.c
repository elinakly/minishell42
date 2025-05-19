/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meta_tokenize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 19:41:33 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/05/13 17:07:42 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/variable.h"
#include "../include/tokenize.h"
#include "../lib/libft/libft.h"

bool	is_escaped_char(char *str, size_t index);
bool	can_escape(char c, e_metachar quot);

/**
 * Checks whether the character at a given index in a string is
 * a shell meta character
 * 
 * Will backtrack to make sure the character is not escaped
 * @param str The string in which to check whether a char is a meta char
 * @param index The index at which the char is positioned
 * @returns `true` if the character is meta, `false` if not
 */
bool	is_meta(char *str, size_t index, e_metachar *meta)
{
	char	c;
	bool	result;

	if (!str)
	{
		if (meta)
			*meta = MC_ERROR;
		return (false);
	}
	c = str[index];
	result = (c == MC_DQUOTE
			|| c == MC_ESCAPE
			|| c == MC_PIPE
			|| c == MC_REDIR_IN
			|| c == MC_REDIR_OUT
			|| c == MC_SQUOTE
			|| c == MC_VARIABLE
			|| c == MC_SEPAR_SPACE
			|| c == MC_SEPAR_TAB)
		&& !is_escaped_char(str, index);
	if (meta && result)
		*meta = (e_metachar)c;
	else if (meta)
		*meta = MC_NONE;
	return (result);
}

/**
 * Checks whether a given metacharacter should start a new token
 * @param meta The metacharacter to check
 * @returns `true` if new token should be started, `false` if not
 */
bool	disrupts_token(e_metachar meta)
{
	return (meta == MC_PIPE || meta == MC_REDIR_IN || meta == MC_REDIR_OUT);
}

/**
 * Checks whether a character would be allowed in an env variable name
 * @param char c The character to check
 * @returns `true` if the character can be used in env vars, `false` if not
 */
bool	is_env_var_char(char c)
{
	return (c == '_' || ft_isalpha(c) || ft_isdigit(c));
}


bool	heredoc_should_expand(char *rawdelim)
{
	size_t		index;
	e_metachar	quot;

	index = 0;
	quot = MC_NONE;
	while (rawdelim[index] && !set_quote_state(rawdelim, index, &quot))
		index++;
	return (!rawdelim[index]);
}


/**
 * Counts the amount of escapable meta characters in a string
 * This differs based on whether the character is in quotes or not
 * @param str The string to check
 * @returns The amount of meta characters in str
 */
size_t	count_esc_metas(t_env_var *var)
{
	size_t		count;
	size_t		index;

	count = 0;
	index = 0;
	if (!var->value)
		return (0);
	while (var->value[index])
	{
		count += is_meta(var->value, index, NULL)
			&& can_escape(var->value[index], var->quote_type);
		index++;
	}
	return (count);
}
