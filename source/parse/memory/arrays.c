/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   arrays.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: Mika Schipper <mschippe@student.codam.n      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/20 01:19:37 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/03/09 15:02:12 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

/**
 * TODO: Rename the folder since this probably won't be exclusively memory stuff
 */

/**
 * Frees every element in an array that ends with a NULL pointer and then
 * frees the actual aray pointer itself. Does NOT set it to NULL.
 * @param arr The array to free (cast any array to void**)
 */
void	free_array(void **arr)
{
	int	i;

	i = 0;
	if (arr == NULL)
		return ;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
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
