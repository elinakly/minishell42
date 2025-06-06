/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmds_count.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 01:19:37 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/05/20 18:03:59 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * TODO: Rename the folder since this probably won't be exclusively memory stuff
 * TODO: Rename this file because this is DEFINITELY not only array stuff
 */

/**
 * Counts the amount of elements in an array
 * Assumes the array is NULL-terminated
 * NULL-terminating element is not included in the length
 */
size_t	ft_arrlen(void **arr)
{
	size_t	count;

	count = 0;
	while (arr && arr[count])
		count++;
	return (count);
}

/**
 * Counts the amount of commands in a t_command linkedlist
 * @param head The first command in the linkedlist
 * @returns The amount of commands
 */
size_t	ft_cmdcount(t_command *head)
{
	size_t	count;

	count = 0;
	while (head)
	{
		count++;
		head = head->next;
	}
	return (count);
}
