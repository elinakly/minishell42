/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   arrays.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: Mika Schipper <mschippe@student.codam.n      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/20 01:19:37 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/02/20 01:44:41 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

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
