/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vars.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mschippe <mschippe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/26 17:06:11 by mschippe      #+#    #+#                 */
/*   Updated: 2025/03/08 13:20:37 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/tokenize.h"
#include "../include/memory.h"
#include "../lib/libft/libft.h"

bool	is_meta(char *str, size_t index, e_metachar *meta);

void	set_quote_state(char *cmd, size_t index, e_metachar *current)
{
	e_metachar	meta;

	if (!cmd)
		return ;
	if (is_meta(cmd, index, &meta)
		&& (meta == MC_SQUOTE || meta == MC_DQUOTE))
	{
		if (*current == MC_NONE)
			*current = meta;
		else if (*current == meta)
			*current = MC_NONE;
	}
}

bool	is_var_char(char c, bool is_first)
{
	return ((c >= '0' && c <= '9' && !is_first)
		|| (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| c == '_');
}

size_t	skip_var_chars(char *cmd, size_t index)
{
	size_t	use_index;

	use_index = index;
	while (is_var_char(cmd[use_index], index == use_index))
		use_index++;
	return (use_index - index);
}

size_t	get_var_count(char *cmd)
{
	size_t		index;
	e_metachar	in_quot;
	size_t		temp_skip;
	size_t		res;

	if (!cmd)
		return (0);
	index = 0;
	res = 0;
	in_quot = MC_NONE;
	while (cmd[index])
	{
		temp_skip = 0;
		set_quote_state(cmd, index, &in_quot);
		if (cmd[index] == '$' && is_meta(cmd, index, NULL))
		{
			temp_skip = skip_var_chars(cmd, ++index);
			res += temp_skip > 0 && in_quot != MC_SQUOTE;
		}
		index += temp_skip;
		if (temp_skip == 0)
			index += 1;
	}
	return (res);
}

size_t	var_len(char *cmd, size_t i)
{
	size_t		quotfind;
	e_metachar	type;
	e_metachar	quottype;

	type = MC_NONE;
	quottype = MC_NONE;
	quotfind = 0;
	if (!cmd || cmd[i] != '$' || !is_meta(cmd, i, &type)
		|| type != MC_VARIABLE)
		return (0);
	while (cmd[quotfind] && quotfind < i)
		set_quote_state(cmd, quotfind++, &quottype);
	if (quottype == MC_SQUOTE)
		return (0);
	return (skip_var_chars(cmd, ++i) + 1);
}

char	**get_var_names(char *cmd)
{
	char	**names;
	size_t	varcount;
	size_t	index;
	size_t	strindex;

	index = 0;
	strindex = 0;
	varcount = get_var_count(cmd);
	names = malloc(sizeof(char *) * (varcount + 1));
	if (!names)
		return (NULL);
	names[varcount] = NULL;
	while (cmd[strindex] && index < varcount)
	{
		if (cmd[strindex] == '$' && var_len(cmd, strindex))
		{
			names[index] = malloc(sizeof(char) * var_len(cmd, strindex));
			if (!names[index])
				return (free_array((void **)names), NULL);
			ft_strlcpy(names[index++], cmd + strindex + 1, var_len(cmd, strindex));
		}
		strindex++;
	}
	return (names);
}
