/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 18:10:39 by eklymova          #+#    #+#             */
/*   Updated: 2025/03/18 19:47:09 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../../lib/libft/libft.h"
#include "../../../include/builtins.h"

int history(char **line)
{
	int fd;
	fd = open(history_fle.txt, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (fd == -1)
		return (1);
	printf(history_file.txt, "%s\n", line);
	return (0);
}