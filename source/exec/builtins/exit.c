/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:54:40 by eklymova          #+#    #+#             */
/*   Updated: 2025/04/29 16:41:57 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include <limits.h>

int	ms_atoi(const char *nptr)
{
	long	sig;
	long	res;

	res = 0;
	sig = 1;
	while ((*nptr > 8 && *nptr < 14) || *nptr == ' ')
		sig = (*nptr++ || res == 0);
	if (*nptr == 43 || *nptr == 45)
		sig = (*nptr++ - 44) * -1;
	while (*nptr >= '0' && *nptr <= '9')
	{
		res = res * 10 + (*nptr++ - 48);
		if (sig * res > INT_MAX || sig * res < INT_MIN)
			return (1);
	}
	return ((int)(sig * res));
}

int	checkdigit(char *argv)
{
	int	i;

	i = 0;
	if (argv[i] == '-' || argv[i] == '+')
		i++;
	if (!argv[i])
		return (0);
	while (argv[i])
	{
		if ((argv[i] == '-' || argv[i] == '+'))
			return (0);
		if (!ft_isdigit(argv[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(char **argv, size_t cmdcount)
{
	long	status;

	// if (cmdcount == 1)
	// 	ft_putstr_fd("exit\n", 2);
	if (!argv[1])
		exit(0);
	if (argv[2])
		return (ft_putstr_fd("minishell: exit: too many arguments\n", 2), 1);
	if (!checkdigit(argv[1]) || ft_strlen(argv[1]) > 19)
	{
		ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
		exit(2);
	}
	status = ms_atoi(argv[1]);
	if (status == 1 && argv[1][0] != '1')
		exit(1);
	if (status > 255 || status < 0)
		status = status % 256;
	exit((unsigned char)(status));
	return (0);
}

