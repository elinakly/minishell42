/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   venv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:00:10 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/05/09 18:44:01 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../../include/venv.h"
#include "../../../include/memory.h"
#include "../../../lib/libft/libft.h"
#include "../../../include/builtins.h"

t_venv	*make_t_venv(bool is_base)
{
	t_venv	*base;

	base = malloc(sizeof(t_venv));
	if (!base)
		return (NULL);
	base->base = is_base;
	base->name = NULL;
	base->value = NULL;
	base->next = NULL;
	return (base);
}

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

t_venv	*make_venv(char **envp)
{
	t_venv	*base;
	t_venv	*temp;
	size_t	index;

	base = make_t_venv(true);
	if (!base)
		return (NULL);
	if (!envp || !envp[0])
		return (base);
	temp = base;
	index = 0;
	while (envp[index])
	{
		temp->next = make_t_venv(false);
		if (!temp->next)
			return (NULL); //TODO: Also needs same free logic
		temp = temp->next;
		if (!set_env_kv(temp, envp[index]))
			return (NULL); //TODO: Free t_venv linkedlist, needs a custom function in memory
		index++;
	}
	return (base);
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
		value = ft_strdup("");
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

void	remove_env_var(t_venv **base, char *name)
{
	t_venv	*temp;
	t_venv	*prev;

	temp = *base;
	prev = NULL;
	while (temp)
	{
		if (!temp->base && ft_strncmp(temp->name, name, ft_strlen(name) + 1) == 0)
		{
			if (prev)
				prev->next = temp->next;
			else
				*base = temp->next;

			free(temp->name);
			free(temp->value);
			free(temp);
			return ;
		}
		prev = temp;
		temp = temp->next;
	}
}

char	*get_env_val(t_shell *shell, char *name)
{
	t_venv	*envp;
	char	res;
	size_t	namelen;
	size_t	varnamelen;

	namelen = ft_strlen(name);
	envp = shell->venv;
	if (ft_strncmp(name, "?", 2) == 0)
		return (ft_itoa(shell->last_status));
	while (envp)
	{
		if (envp->base)
		{
			envp = envp->next;
			continue ;
		}
		varnamelen = ft_strlen(envp->name);
		if (namelen > varnamelen)
			varnamelen = namelen;
		if (ft_strncmp(name, envp->name, varnamelen + 1) == 0)
			return (ft_strdup(envp->value));
		envp = envp->next;
	}
	return (NULL);
}

char	*env_var_str(t_venv *var)
{
	char	*res;
	size_t	total_len;
	size_t	resindex;
	size_t	strindex;

	if (var->base)
		return (NULL);
	total_len = ft_strlen(var->name) + ft_strlen(var->value) + 2;
	resindex = 0;
	strindex = 0;
	res = malloc(total_len * sizeof(char));
	if (!res)
		return (NULL);
	res[total_len - 1] = '\0';
	while (var->name[strindex])
		res[resindex++] = var->name[strindex++];
	strindex = 0;
	res[resindex++] = '=';
	while (var->value[strindex])
		res[resindex++] = var->value[strindex++];
	return (res);
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
