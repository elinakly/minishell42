/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmds_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:52:35 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/20 18:30:21 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Frees every element in an array that ends with a NULL pointer and then
 * frees the actual aray pointer itself. Does NOT set it to NULL.
 * Also takes an optional function pointer that it will call to delete
 * whatever is inside of the array elements
 * @param arr The array to free (cast any array to void**)
 * @param del The function to call to delete what's inside elements (nullable)
 */
void	free_array(void **arr, void (*del)(void *))
{
	int	i;

	i = 0;
	if (arr == NULL)
		return ;
	while (arr[i] != NULL)
	{
		if (del)
			del(arr[i]);
		free(arr[i]);
		i++;
	}
	free(arr);
}

/**
 * Frees a redirect and all its allocated fields
 * @param redir The redirect to free
 */
void	free_single_redir(t_redirect *redir)
{
	if (!redir)
		return ;
	free(redir->file);
	free(redir->heredoc_delim);
	free(redir);
}

/**
 * Frees all redirects in a linkedlist
 * @param head The first redirect in the linkedlist
 */
void	free_redirs(t_redirect *head)
{
	t_redirect	*next;

	while (head)
	{
		next = head->next;
		free_single_redir(head);
		head = next;
	}
}

/**
 * Frees a command struct and all its allocated fields
 * @param cmd The command to free
 */
void	free_single_cmd(t_command *cmd)
{
	if (!cmd)
		return ;
	free(cmd->name);
	free_redirs(cmd->redirects);
	free_array((void **)cmd->argv, NULL);
	free(cmd);
}

/**
 * Frees all command structs in a linked list
 * @param head The first command struct in the list
 */
void	free_commands(t_command *head)
{
	t_command	*next;

	while (head)
	{
		next = head->next;
		free_single_cmd(head);
		head = next;
	}
}
