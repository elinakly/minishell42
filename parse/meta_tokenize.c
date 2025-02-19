/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meta_tokenize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 19:41:33 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/02/19 15:23:37 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "tokenize.h"

int	skip_quoted(char *str);
bool	is_escaped_char(char *str, size_t index);

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

	c = str[index];
	result = (c == MC_DQUOTE
		|| c == MC_ESCAPE
		|| c == MC_PIPE
		|| c == MC_REDIR_IN
		|| c == MC_REDIR_OUT
		|| c == MC_SQUOTE
		|| c == MC_VARIABLE)
		&& !is_escaped_char(str, index);
	if (meta && result)
		*meta = (e_metachar)c;
	else if (meta)
		*meta = MC_NONE;
	return (result);
}

bool	disrupts_token(e_metachar meta)
{
	return (meta == MC_PIPE || meta == MC_REDIR_IN || meta == MC_REDIR_OUT);
}

/**
 * TODO: Replace with libft function
 */
bool	is_alpha(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

/**
 * TODO: Replace with libft function use
 */
bool	is_num(char c)
{
	return (c >= '0' && c <= '9');
}

/**
 * Checks whether a character would be allowed in an env variable name
 * @param char c The character to check
 * @returns `true` if the character can be used in env vars, `false` if not
 */
bool	is_env_var_char(char c)
{
	return (c == '_' || is_alpha(c) || is_num(c));
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
	if (*str == MC_PIPE)
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
