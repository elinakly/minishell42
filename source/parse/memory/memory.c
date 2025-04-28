/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   memory.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mschippe <mschippe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/20 01:19:37 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/04/28 17:06:34 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "../../include/tokenize.h"
#include "../../include/structbuild.h"
#include "../../include/path.h"

/**
 * TODO: Rename the folder since this probably won't be exclusively memory stuff
 * TODO: Rename this file because this is DEFINITELY not only array stuff
 */

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
 * Frees the data inside of a partial environment variable
 * @param var The partial variable to free data in (cast to void pointer)
 */
void	clear_part_var(void *var)
{
	t_part_var *realvar = (t_part_var *)var;
	if (var)
		free(realvar->name);
}

/**
 * Deletes the name and value strings inside of an environment variable struct
 * @param var The variable struct (cast to a void pointer) to target
 */
void	clear_env_var(void *var)
{
	t_env_var *realvar = (t_env_var *)var;
	if (var)
	{
		free(realvar->name);
		free(realvar->value);
	}
}

/**
 * Frees a single token and all its allocated fields
 * @param token The token to free
 */
void	free_single_token(t_token *token)
{
	if (!token)
		return ;
	free(token->raw_value);
	free(token->value);
	free(token);
}

/**
 * Loops through token linkedlist and frees all of them
 * @param head The first token in the linkedlist
 */
void	free_tokens(t_token *head)
{
	t_token	*next;

	while (head)
	{
		next = head->next;
		free_single_token(head);
		head = next;
	}
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

void	free_venv(t_venv *base)
{
	t_venv	*next;

	if (!base)
		return ;
	while (base)
	{
		next = base->next;
		if (!base->base)
		{
			free(base->name);
			free(base->value);
		}
		free(base);
		base = next;
	}
}

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
