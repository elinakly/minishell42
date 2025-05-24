/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   venv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:00:10 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/05/24 16:39:01 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			return (NULL);
		temp = temp->next;
		if (!set_env_kv(temp, envp[index]))
			return (NULL);
		index++;
	}
	return (base);
}

void	remove_env_var(t_venv **base, char *name)
{
	t_venv	*temp;
	t_venv	*prev;

	temp = *base;
	prev = NULL;
	while (temp)
	{
		if (!temp->base && ft_strncmp(temp->name, name,
				ft_strlen(name) + 1) == 0)
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
	size_t	namelen;
	size_t	varnamelen;

	namelen = ft_strlen(name);
	envp = shell->venv;
	if (ft_strncmp(name, "?", 2) == 0)
		return (ft_itoa(shell->status));
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
