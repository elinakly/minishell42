/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   venv.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: Mika Schipper <mschippe@student.codam.n      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/04/14 17:00:10 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/04/16 19:00:06 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../../include/venv.h"
#include "../../../include/memory.h"
#include "../../../lib/libft/libft.h"

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
		return(false);
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

	new = malloc(sizeof(t_venv));
	if (!new)
		return (false);
	if (!set_env_kv(new, strvar))
		return (free_venv(new), false);
	base->next = new;
	return (true);
}

void	remove_env_var(t_venv *base, char *name)
{
	size_t	index;
	size_t	counter;
	t_venv	*temp;
	t_venv	*prev;

	index = 0;
	counter = 0;
	temp = base;
	prev = base;
	while (temp)
	{
		if (ft_strncmp(base->name, name, ft_strlen(name) + 1) == 0)
			break;
		index++;
		temp = base;
	}
	if (!temp)
		return ;
	while (counter++ < index)
		prev = prev->next;
	prev->next = temp->next;
	free(temp->name);
	free(temp->value);
	free(temp);
}

char	*get_env_val(t_venv *envp, char *name)
{
	char	res;

	while (envp)
	{
		if (envp->base)
		{
			envp = envp->next;
			continue ;
		}
		if (ft_strncmp(name, envp->name, ft_strlen(name) + 1) == 0)
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
