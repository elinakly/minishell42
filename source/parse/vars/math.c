/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:53:01 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/20 18:40:22 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Returns the size of an environment variable inside a string
 * similar to `skip_var_chars` except this includes all the
 * necessary validation to ensure we are actually dealing with
 * a variable
 * @param cmd The command string to count variable length in
 * @param i The index at which to start counting
 * @returns Length of an environment variable name
 */
size_t	var_len(char *cmd, size_t i, bool track_quotes)
{
	size_t		quotfind;
	t_metachar	type;
	t_metachar	quottype;

	type = MC_NONE;
	quottype = MC_NONE;
	quotfind = 0;
	if (!cmd || cmd[i] != '$' || !is_meta(cmd, i, &type)
		|| type != MC_VARIABLE)
		return (0);
	if (track_quotes)
	{
		while (cmd[quotfind] && quotfind < i)
			set_quote_state(cmd, quotfind++, &quottype);
	}
	if (quottype == MC_SQUOTE)
		return (0);
	return (skip_var_chars(cmd, ++i) + 1);
}

/**
 * Calculates what the new command string size would be after
 * expanding all the variables inside of it
 * 
 * Considers the need for backslashes in front of meta-
 * characters inside of variable values
 * @param cmd The command string
 * @param vars The array of environment variables to expand
 * @returns The expanded string's length
 */
size_t	calc_expanded_len(char *cmd, t_env_var **vars)
{
	size_t	orig;
	size_t	names_len;
	size_t	values_len;
	size_t	index;

	orig = ft_strlen(cmd);
	names_len = 0;
	values_len = 0;
	index = 0;
	if (!cmd || !vars)
		return (0);
	while (vars[index])
	{
		names_len += ft_strlen(vars[index]->name) + 1;
		values_len += ft_strlen(vars[index]->value)
			+ count_esc_metas(vars[index]);
		index++;
	}
	return (orig - names_len + values_len);
}

/**
 * Calculates how many environment variables are in a command string
 * Considers quotes, escapes, etc. in its calculation
 * TODO: `<<$` seemingly creates no tokens at all right now, investigate why
 * TODO: The above also happens with stuff like
 * 	`echo hey | $` (ending empty dollarsign)
 * c = cmd
 * i = index
 * 
 * TODO: Diff from above TODOs: try skipping expansion
 * 	for vars coming after heredoc MAYBE?
 * @param cmd The command string to count variables in
 * @returns The amount of environment variables in the string
 */
size_t	get_var_count(char *c, bool track_quotes)
{
	size_t		i;
	t_metachar	in_quot;
	size_t		temp_skip;
	size_t		res;

	if (!c)
		return (0);
	i = 0;
	res = 0;
	in_quot = MC_NONE;
	while (c[i])
	{
		temp_skip = 0;
		if (track_quotes)
			set_quote_state(c, i, &in_quot);
		if (c[i] == '$' && is_meta(c, i, 0) && (c[i + 1] && c[i + 1] != '\n'))
		{
			temp_skip = skip_var_chars(c, ++i);
			res += temp_skip > 0 && in_quot != MC_SQUOTE;
		}
		i += temp_skip;
		if (temp_skip == 0)
			i += 1;
	}
	return (res);
}
