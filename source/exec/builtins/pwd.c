/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 18:46:43 by eklymova          #+#    #+#             */
/*   Updated: 2025/03/13 20:03:26 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	pwd()
{
	char *buff;

	buff = getcwd(NULL, PATH_MAX);
	if (buff)
	{
		printf("%s\n", buff);
		free(buff);
		return (0);
	}
	return (1);
}
