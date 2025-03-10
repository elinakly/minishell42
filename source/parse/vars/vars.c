/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   vars.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mschippe <mschippe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/26 17:06:11 by mschippe      #+#    #+#                 */
/*   Updated: 2025/03/09 23:53:35 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../include/tokenize.h"
#include "../include/memory.h"
#include "../include/variable.h"
#include "../lib/libft/libft.h"

bool	is_meta(char *str, size_t index, e_metachar *meta);
size_t	count_metas(char *str);

/**
 * Updates a pointer to a metachar enum based on its previous state
 * This is used to easily keep track of whether we are currently
 * inside quotes!
 * @param cmd A string holding presumably a minishell command
 * @param index The index of the character to check
 * @param current Pointer to current quote state to be updated
 */
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

/**
 * Returns whether a character is a valid character for an
 * environment variable name
 * @param c The character to check
 * @param is_first Whether the character is the first in a variable name
 * @returns `true` if character is valid, `false` if not
 */
bool	is_var_char(char c, bool is_first)
{
	return ((c >= '0' && c <= '9' && !is_first)
		|| (c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| c == '_');
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
	while (is_var_char(cmd[use_index], index == use_index))
		use_index++;
	return (use_index - index);
}

/**
 * Calculates how many environment variables are in a command string
 * Considers quotes, escapes, etc. in its calculation
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
 * Gets all the environment variable names in a command string
 * Allocates memory for each one and puts them in an array
 * @param cmd The command string to find variable names in
 * @returns a NULL-terminated array of strings containing variable names
 */
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
				return (free_array((void **)names, NULL), NULL);
			ft_strlcpy(names[index++], cmd + strindex + 1,
				var_len(cmd, strindex));
		}
		strindex++;
	}
	return (names);
}

/**
 * Uses environment variables and a variable name to get its value
 * And then turn that into a malloced environment variable struct
 * TODO: Get envp from some global big struct instead of passing it around
 * @param envp Array containing all environment variables
 * @param name The environment variable name
 * @returns Malloced environment variable struct holding name and value strings
 */
t_env_var *make_var(char *name)
{
	t_env_var	*var;
	char		*value;

	if (!name)
		return (NULL);
	var = malloc(sizeof(t_env_var));
	if (!var)
		return (NULL);
	var->name = ft_strdup(name);
	value = getenv(name);
	if (!value)
		var->value = ft_strdup("");
	else
		var->value = ft_strdup(value);
	if (!var->value)
		return (free(var), NULL); // TODO: Make sure we really don't want to free name here (but probably not)
	return (var);
}

/**
 * Creates an array of environment variable structs based on the
 * given environment variables and variable names
 * 
 * NOTE: It makes copies of variable names and values, so those must be freed!
 * TODO: See the commented line inside the function for info on TODO
 * @param envp Array containing all environment variables
 * @param names Array containing variable names that we want to create
 * @returns An array of environment variable structs
 */
t_env_var	**get_command_vars(char **names)
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
		return (NULL);
	vars[amount] = NULL;
	while (index < amount)
	{
		vars[index] = make_var(names[index]);
		if (!vars[index])
			return (NULL); // TODO: Update free_arr function to take a del function pointer and call it here to free! CURRENTLY LEAKS!
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
size_t calc_expanded_len(char *cmd, t_env_var **vars)
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
			+ count_metas(vars[index]->value);
		index++;
	}
	return (orig - names_len + values_len);
}

/**
 * Turns an environment variable value into its escaped version
 * It will allocate enough memory to add an escape character
 * in front of every meta character
 * @param value The environment variable value
 * @returns An escaped version of `value`
 */
char *get_escaped_value(char *value)
{
	size_t	size;
	size_t	resindex;
	size_t	valueindex;
	char	*res;

	if (!value)
		return (NULL);
	resindex = 0;
	valueindex = 0;
	size = ft_strlen(value) + count_metas(value);
	res = malloc(sizeof(char) * (size + 1));
	if (!res)
		return (NULL);
	while (value[valueindex])
	{
		if (is_meta(value, resindex, NULL))
			res[resindex++] = '\\';
		res[resindex++] = value[valueindex++];
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
	temp = get_escaped_value(var->value);
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
 * TODO: Break up and norminette fix
 * @param cmd The raw command string to create an expanded version of
 * @param vars The variables to find and expand
 */
char	*get_expanded_cmd(char *cmd, t_env_var **vars)
{
	char		*res;
	struct		indices;
	size_t		resindex;
	size_t		cmdindex;
	size_t		varindex;
	e_metachar	quot;
	e_metachar	meta;

	quot = MC_NONE;
	resindex = 0;
	cmdindex = 0;
	varindex = 0;
	res = ft_calloc(calc_expanded_len(cmd, vars) + 1, sizeof(char));
	if (!res)
		return (NULL);
	while (cmd[cmdindex])
	{
		set_quote_state(cmd, cmdindex, &quot);
		if (is_meta(cmd, cmdindex, &meta) && meta == MC_VARIABLE && quot != MC_SQUOTE)
		{
			if (!insert_var(res, vars[varindex], &resindex))
				return (free(res), NULL);
			cmdindex += ft_strlen(vars[varindex++]->name) + 1;
		}
		else
			res[resindex++] = cmd[cmdindex++];
	}
	res[resindex] = '\0';
	return (res);
}
