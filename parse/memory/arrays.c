/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   arrays.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: Mika Schipper <mschippe@student.codam.n      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/20 01:19:37 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/02/20 01:25:17 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

/**
 * Frees every element in an array that ends with a NULL pointer and then
 * frees the actual aray pointer itself. Does NOT set it to NULL.
 * @param arr The array to free (cast any array to void**)
 */
void	free_array(void **arr)
{
	if (!arr)
		return ;
	while (*arr)
		free(*arr++);
	free(arr);
}
