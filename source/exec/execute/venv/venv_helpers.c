/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   venv_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:37:36 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/24 17:14:09 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	set_env_kv(t_venv *var, char *strvar)
{
	char	**res;

	if (!var || !strvar)
		return (false);
	if (!ft_strchr(strvar, '='))
		return (false);
	res = ft_split(strvar, '=');
	if (!res || !res[0])
		return (false);
	var->name = ft_strdup(res[0]);
	if (!var->name)
		return (free_array((void **)res, NULL), false);
	if (res[1])
		var->value = ft_strdup(res[1]);
	else
		var->value = ft_strdup("");
	if (!var->value)
		return (free_array((void **)res, NULL), free(var->name), false);
	free_array((void **)res, NULL);
	return (true);
}

bool	add_env_var(t_venv *base, char *strvar)
{
	t_venv	*new;

	while (base->next)
		base = base->next;
	new = make_t_venv(false);
	if (!new)
		return (false);
	if (!set_env_kv(new, strvar))
		return (free_venv(new), false);
	base->next = new;
	return (true);
}

bool	set_name_value(t_venv *var, char *name, char *value)
{
	char	*tmpname;
	char	*tmpvalue;

	tmpname = var->name;
	tmpvalue = var->value;
	if (!value)
	{
		value = "";
		if (!value)
			return (false);
	}
	var->name = ft_strdup(name);
	if (!var->name)
	{
		var->name = tmpname;
		return (false);
	}
	var->value = ft_strdup(value);
	if (!var->value)
	{
		var->name = tmpname;
		var->value = tmpvalue;
		return (false);
	}
	return (free(tmpname), free(tmpvalue), true);
}

bool	simple_add_var(t_venv *base, char *name, char *value)
{
	t_venv	*cpy;

	cpy = base;
	while (cpy)
	{
		if (cpy->name && strequals(name, cpy->name))
			break ;
		cpy = cpy->next;
	}
	if (cpy)
		return (set_name_value(cpy, name, value));
	cpy = make_t_venv(false);
	if (!cpy || !set_name_value(cpy, name, value))
		return (free(cpy), false);
	while (base->next)
		base = base->next;
	base->next = cpy;
	return (true);
}

char	**venv_to_arr(t_venv *base)
{
	size_t	count;
	char	**res;
	t_venv	*temp;

	temp = base;
	count = 0;
	while (temp->next)
	{
		count++;
		temp = temp->next;
	}
	res = malloc(sizeof(char *) * (count + 1));
	if (!res)
		return (NULL);
	res[count] = NULL;
	count = 0;
	while (base->next)
	{
		res[count] = env_var_str(base->next);
		if (!res[count])
			return (free_array((void **)res, NULL), NULL);
		count++;
		base = base->next;
	}
	return (res);
}
