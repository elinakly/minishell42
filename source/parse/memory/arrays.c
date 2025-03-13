/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   arrays.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: mschippe <mschippe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/20 01:19:37 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/03/13 01:37:33 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "../../include/tokenize.h"
#include "../../include/structbuild.h"

/**
 * TODO: Rename the folder since this probably won't be exclusively memory stuff
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

void	clear_token_var(void *var)
{
	t_token *token = (t_token *)var;
	if (var)
		free(token->value);
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
