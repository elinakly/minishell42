/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:06:11 by mschippe          #+#    #+#             */
/*   Updated: 2025/05/13 16:53:30 by eklymova         ###   ########.fr       */
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
 * Gets all the environment variable names and quote types in a command
 * Requires you to have an array allocated already that you 
 * pass into this function (names arg)
 * @param cmd The command string to find variable names in
 * @param varcount The amount of variables the array will have
 * @param names The array in which to put all the variable data
 * @returns a NULL-terminated array of partial env vars (name + quote type)
 */
t_part_var	**get_var_names(char *cmd, size_t varcount,
							t_part_var **names, bool t)
{
	size_t		i;
	size_t		si;
	e_metachar	quot;

	i = 0;
	si = 0;
	quot = MC_NONE;
	while (cmd[si] && i < varcount)
	{
		if (t)
			set_quote_state(cmd, si, &quot);
		if (cmd[si] == '$' && var_len(cmd, si, t) > 1)
		{
			names[i] = malloc(sizeof(t_part_var));
			if (!names[i])
				return (free_array((void **)names, &clear_part_var), NULL);
			names[i]->name = malloc(sizeof(char) * var_len(cmd, si, t));
			if (!names[i]->name)
				return (free_array((void **)names, &clear_part_var), NULL);
			ft_strlcpy(names[i]->name, cmd + si + 1, var_len(cmd, si, t));
			names[i++]->in_quote_type = quot;
		}
		si++;
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
 * Turns a raw command string into one which has all its
 * environment variables expanded
 * @param cmd The raw command string to create an expanded version of
 * @param vars The variables to find and expand
 */
char	*get_expanded_cmd(char *cmd, t_env_var **vars, bool track_quotes)
{
	char			*res;
	t_triple_index	i;
	e_metachar		quot;
	e_metachar		meta;

	quot = MC_NONE;
	i = (t_triple_index){0, 0, 0};
	res = ft_calloc(calc_expanded_len(cmd, vars) + 1, sizeof(char));
	if (!res)
		return (NULL);
	while (cmd[i.cmd])
	{
		if (track_quotes)
			set_quote_state(cmd, i.cmd, &quot);
		if (is_meta_var(cmd, i, &meta, &quot))
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
