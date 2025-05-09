/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 18:46:59 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/09 18:47:00 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../include/builtins.h"

int	env(char **envp)
{
	while (*envp)
	{
		printf("%s\n", *envp);
		envp++;
	}
	return (0);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	(void)argc;
// 	(void)argv;
// 	env(envp);
// 	return (0);
// }