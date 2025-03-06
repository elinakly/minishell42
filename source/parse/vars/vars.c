/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:06:11 by mschippe          #+#    #+#             */
/*   Updated: 2025/03/06 18:57:36 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/tokenize.h"

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
	temp_skip = 0;
	res = 0;
	in_quot = MC_NONE;
	while (cmd[index])
	{
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
	size_t		res;
	e_metachar	type;

	res = 1;
	type = MC_NONE;
	if (!cmd || !cmd[i] || cmd[i] != '$' || !is_meta(cmd, i, &type)
		|| type != MC_VARIABLE);
		return (0);
	return (res + skip_var_chars(cmd, ++i));
}

char	**get_var_names(char *cmd)
{
	size_t	varcount;
	char	**names;

	varcount = get_var_count(cmd);
	names = malloc(sizeof(char *) * (varcount + 1));
	if (!names)
		return (NULL);
}