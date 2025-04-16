/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   venv.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: Mika Schipper <mschippe@student.codam.n      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/04/14 17:00:10 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/04/16 13:03:43 by Mika Schipp   ########   odam.nl         */
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

char	*get_env(t_venv *envp, char *name)
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
