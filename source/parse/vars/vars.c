/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:06:11 by mschippe          #+#    #+#             */
/*   Updated: 2025/05/07 10:50:28 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../../include/variable.h"
#include "../../../include/tokenize.h"
#include "../../../include/memory.h"
#include "../../../include/venv.h"
#include "../../../lib/libft/libft.h"

bool	is_meta(char *str, size_t index, e_metachar *meta);
bool	can_escape(char c, e_metachar quot);

/**
 * Updates a pointer to a metachar enum based on its previous state
 * This is used to easily keep track of whether we are currently
 * inside quotes!
 * @param cmd A string holding presumably a minishell command
 * @param index The index of the character to check
 * @param current Pointer to current quote state to be updated
 */
bool	set_quote_state(char *cmd, size_t index, e_metachar *current)
{
	e_metachar	meta;
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

/**
 * Calculates how many environment variables are in a command string
 * Considers quotes, escapes, etc. in its calculation
 * TODO: `<<$` seemingly creates no tokens at all right now, investigate why
 * TODO: The above also happens with stuff like `echo hey | $` (ending empty dollarsign)
 * 
 * TODO: Diff from above TODOs: try skipping expansion for vars coming after heredoc MAYBE?
 * @param cmd The command string to count variables in
 * @returns The amount of environment variables in the string
 */
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
		if (cmd[index] == '$' && is_meta(cmd, index, NULL) && cmd[index + 1])
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

/**
 * Returns the size of an environment variable inside a string
 * similar to `skip_var_chars` except this includes all the
 * necessary validation to ensure we are actually dealing with
 * a variable
 * @param cmd The command string to count variable length in
 * @param i The index at which to start counting
 * @returns Length of an environment variable name
 */
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

/**
 * Gets all the environment variable names and quote types in a command
 * Requires you to have an array allocated already that you 
 * pass into this function (names arg)
 * @param cmd The command string to find variable names in
 * @param varcount The amount of variables the array will have
 * @param names The array in which to put all the variable data
 * @returns a NULL-terminated array of partial env vars (name + quote type)
 */
t_part_var	**get_var_names(char *cmd, size_t varcount, t_part_var **names)
{
	size_t		i;
	size_t		str_i;
	e_metachar	quot;

	i = 0;
	str_i = 0;
	quot = MC_NONE;
	while (cmd[str_i] && i < varcount)
	{
		set_quote_state(cmd, str_i, &quot);
		if (cmd[str_i] == '$' && var_len(cmd, str_i))
		{
			names[i] = malloc(sizeof(t_part_var));
			if (!names[i])
				return (free_array((void **)names, &clear_part_var), NULL);
			names[i]->name = malloc(sizeof(char) * var_len(cmd, str_i));
			if (!names[i]->name)
				return (free_array((void **)names, &clear_part_var), NULL);
			ft_strlcpy(names[i]->name, cmd + str_i + 1, var_len(cmd, str_i));
			names[i++]->in_quote_type = quot;
		}
		str_i++;
	}
	return (names);
}

/**
 * Uses environment variables and a variable name to get its value
 * And then turn that into a malloced environment variable struct
 * @param envp Array containing all environment variables
 * @param part Partial environment variable (name + quote type)
 * @returns Malloced environment variable struct holding name and value strings
 */
t_env_var	*make_var(t_part_var *part, t_shell *shell)
{
	t_env_var	*var;
	char		*value;

	if (!part)
		return (NULL);
	var = malloc(sizeof(t_env_var));
	if (!var)
		return (NULL);
	var->name = ft_strdup(part->name);
	var->quote_type = part->in_quote_type;
	value = get_env_val(shell, part->name); // TODO: If strdup is used to copy this below, maybe we need to free this here
	if (!value)
		var->value = ft_strdup("");
	else
		var->value = ft_strdup(value); // TODO: May not need strdup
	if (!var->value)
		return (free(var), NULL); // TODO: Make sure we really don't want to free name here (but probably not)
		return (var);
}

/**
 * Creates an array of environment variable structs based on the
 * given environment variables and variable names
 * 
 * NOTE: It makes copies of variable names and values, so those must be freed!
 * @param envp Array containing all environment variables
 * @param names Array containing partial env vars (name + quote type)
 * @returns An array of environment variable structs
 */
t_env_var	**get_command_vars(t_part_var **names, t_shell *shell)
{
	t_env_var	**vars;
	size_t		amount;
	size_t		index;

	amount = ft_arrlen((void **)names);
	index = 0;
	if (!names)
		return (NULL);
	vars = malloc(sizeof(t_env_var *) * (amount + 1));
	if (!vars)
		return (free_array((void **)names, &clear_part_var), NULL);
	vars[amount] = NULL;
	while (index < amount)
	{
		vars[index] = make_var(names[index], shell);
		if (!vars[index])
			return (free_array((void **)vars, &clear_env_var),
					free_array((void **)names, &clear_part_var), NULL);
		index++;
	}
	return (vars);
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
 * Turns an environment variable value into its escaped version
 * It will allocate enough memory to add an escape character
 * in front of every meta character
 * @param value The environment variable value
 * @param quot The type of quote this variable was inside of
 * @returns An escaped version of `value`
 */
char	*get_escaped_value(t_env_var *var)
{
	size_t	size;
	size_t	resindex;
	size_t	v_index;
	char	*res;

	if (!var || !var->value)
		return (NULL);
	resindex = 0;
	v_index = 0;
	size = ft_strlen(var->value) + count_esc_metas(var);
	res = malloc(sizeof(char) * (size + 1));
	if (!res)
		return (NULL);
	while (var->value[v_index])
	{
		if (is_meta(var->value, v_index, NULL)
			&& can_escape(var->value[v_index], var->quote_type))
			res[resindex++] = '\\';
		res[resindex++] = var->value[v_index++];
	}
	res[resindex] = '\0';
	return (res);
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
 * Turns a raw command string into one which has all its
 * environment variables expanded
 * @param cmd The raw command string to create an expanded version of
 * @param vars The variables to find and expand
 */
char	*get_expanded_cmd(char *cmd, t_env_var **vars)
{
	char		*res;
	t_triple_index i;
	e_metachar	quot;
	e_metachar	meta;

	quot = MC_NONE;
	i = (t_triple_index){0, 0, 0};
	res = ft_calloc(calc_expanded_len(cmd, vars) + 1, sizeof(char));
	if (!res)
		return (NULL);
	while (cmd[i.cmd])
	{
		set_quote_state(cmd, i.cmd, &quot);
		if (is_meta(cmd, i.cmd, &meta) && meta == MC_VARIABLE
			&& quot != MC_SQUOTE && is_var_char(cmd, i.cmd + 1))
		{
			if (!insert_var(res, vars[i.var], &i.res))
				return (free(res), NULL);
			i.cmd += ft_strlen(vars[i.var++]->name) + 1;
		}
		else
			res[i.res++] = cmd[i.cmd++];
	}
	res[i.res] = '\0';
	return (res);
}
