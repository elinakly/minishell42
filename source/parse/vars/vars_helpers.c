/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:53:10 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/20 18:16:21 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Returns whether a character is a valid character for an
 * environment variable name
 * @param c The character to check
 * @param is_first Whether the character is the first in a variable name
 * @returns `true` if character is valid, `false` if not
 */
bool	is_var_char(char *str, size_t index)
{
	bool	is_first;
	size_t	index_cpy;

	is_first = index > 0 && str[index - 1] == '$';
	index_cpy = index;
	if (is_first && str[index] == '?')
		return (true);
	if (!is_first)
	{
		while (index_cpy >= 0 && str[index_cpy] != '$')
			index_cpy--;
		if (str[index_cpy] == '$' && str[index_cpy + 1] == '?'
			&& index_cpy < index)
			return (false);
	}
	return ((str[index] >= '0' && str[index] <= '9' && !is_first)
		|| (str[index] >= 'a' && str[index] <= 'z')
		|| (str[index] >= 'A' && str[index] <= 'Z')
		|| str[index] == '_');
}

/**
 * Returns the size of an environment variable name (after the '$')
 * @param cmd The string in which to check (usually a command)
 * @param index The index at which to start checking
 * @returns Length of an environment variable name
 */
size_t	skip_var_chars(char *cmd, size_t index)
{
	size_t	use_index;

	use_index = index;
	while (is_var_char(cmd, use_index))
		use_index++;
	return (use_index - index);
}

bool	is_meta_var(char *cmd, t_triple_index i, t_metachar *meta,
	t_metachar *quot)
{
	return (is_meta(cmd, i.cmd, meta) && *meta == MC_VARIABLE
		&& *quot != MC_SQUOTE && is_var_char(cmd, i.cmd + 1));
}

/**
 * Inserts a variable value into a command string
 * @param res The string to which the value should be written
 * @param var The environment variable struct to use
 * @param index The index at which the variable should be inserted
 */
bool	insert_var(char *res, t_env_var *var, size_t *index)
{
	char	*temp;
	size_t	templen;

	if (!res || !var || !var->name || !var->value || !index)
		return (false);
	temp = get_escaped_value(var);
	if (!temp)
		return (false);
	templen = ft_strlen(temp);
	ft_strlcat(res, temp, ft_strlen(res) + templen + 1);
	*index += templen;
	free(temp);
	return (true);
}

/**
 * Updates a pointer to a metachar enum based on its previous state
 * This is used to easily keep track of whether we are currently
 * inside quotes!
 * @param cmd A string holding presumably a minishell command
 * @param index The index of the character to check
 * @param current Pointer to current quote state to be updated
 */
bool	set_quote_state(char *cmd, size_t index, t_metachar *current)
{
	t_metachar	meta;

	if (!cmd)
		return (false);
	if (is_meta(cmd, index, &meta)
		&& (meta == MC_SQUOTE || meta == MC_DQUOTE))
	{
		if (*current == MC_NONE)
		{
			*current = meta;
			return (true);
		}
		else if (*current == meta)
		{
			*current = MC_NONE;
			return (true);
		}
		return (false);
	}
	return (false);
}
