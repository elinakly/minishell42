/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   meta_tokenize.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mschippe <mschippe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/17 19:41:33 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/03/12 04:16:13 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/variable.h"
#include "../include/tokenize.h"
#include "../lib/libft/libft.h"

int		skip_quoted(char *str);
bool	is_escaped_char(char *str, size_t index);
bool	can_escape(char c, e_metachar quot);
bool	set_quote_state(char *cmd, size_t index, e_metachar *current);

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
		|| c == MC_ARG_SEPARATE)
		&& !is_escaped_char(str, index);
	if (meta && result)
		*meta = (e_metachar)c;
	else if (meta)
		*meta = MC_NONE;
	return (result);
}

bool	is_disrupt_meta(char *str, size_t index, e_metachar *meta)
{
	bool		is_any_meta;
	e_metachar	type;

	type = MC_NONE;
	is_any_meta = is_meta(str, index, &type);
	if (meta)
		*meta = type;
	return (is_any_meta && type == MC_REDIR_IN
		|| type == MC_REDIR_OUT || type == MC_PIPE);
}

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

/**
 * Calculates the length of an environment variable token
 * @param str The string in which to look (can be offset)
 * @returns The amount of characters to move forward in order to skip the token
 */
size_t	skip_env_var(char *str)
{
	size_t	count;
	if (!str || *str != MC_VARIABLE)
		return (0);
	count = 1;
	while (is_env_var_char(str[count]))
		count++;
	return (count);
}

/**
 * Calculates the length of an input redirection token
 * 
 * Works for both a regular redirection and heredoc (basic, no quote support)
 * @param str The string in which to look (can be offset)
 * @returns The amount of characters to move forward in order to skip the token
 */
size_t	skip_redir_in(char *str)
{
	size_t	count;

	if (!str || *str != MC_REDIR_IN)
		return (0);
	if (*(str + 1) != MC_REDIR_IN)
		return (1);
	count = 2;
	while (is_env_var_char(str[count]))
		count++;
	return (count);
}

/**
 * Calculates the length of any supported meta character token
 * @param str The string in which to look (can be offset)
 * @returns The amount of characters to move forward in order to skip the token
 */
size_t	skip_meta(char *str)
{
	if (!str)
		return (0);
	if (*str == MC_PIPE || *str == MC_ARG_SEPARATE)
		return (1);
	if (*str == MC_ESCAPE)
		return (2);
	if (*str == MC_DQUOTE || *str == MC_SQUOTE)
		return (skip_quoted(str));
	if (*str == MC_REDIR_OUT)
		return (1 + (*(str + 1) == MC_REDIR_OUT));
	if (*str == MC_VARIABLE)
		return (skip_env_var(str));
	if (*str == MC_REDIR_IN)
		return (skip_redir_in(str));
	return (0);
}

/**
 * Counts the amount of escapable meta characters in a string
 * This differs based on whether the character is in quotes or not
 * @param str The string to check
 * @returns The amount of meta characters in str
 */
size_t	count_esc_metas(char *str)
{
	size_t		count;
	size_t		index;
	e_metachar	quot;

	count = 0;
	index = 0;
	quot = MC_NONE;
	if (!str)
		return (0);
	while (str[index])
	{
		set_quote_state(str, index, &quot);
		count += is_meta(str, index, NULL) && can_escape(str[index], quot);
		index++;
	}
	return (count);
}
