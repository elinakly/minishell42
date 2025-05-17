/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:54:40 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/17 17:09:07 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtins.h"
#include "../../../lib/libft/libft.h"

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

int	not_so_fake_exit(t_shell *shell, int code)
{
	shell->last_status = code;
	shell->loop_active = false;
	return (code);
}

int	ft_exit(t_shell *shell, char **argv)
{
	unsigned long long	status;

	// if (shell->cmds_count == 1)
	// 	ft_putstr_fd("exit\n", 2);
	if (!argv[1])
		return (not_so_fake_exit(shell, 0));
	if (argv[2])
		return (ft_putstr_fd("minishell: exit: too many arguments\n", 2), 1);
	if (!checkdigit(argv[1]) || ft_strlen(argv[1]) > 20)
	{
		ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
		return (not_so_fake_exit(shell, 2));
	}
	status = ft_atoi(argv[1]);
	if (status == 1 && argv[1][0] != '1')
		return (not_so_fake_exit(shell, 1));
	if (status > 255 || status < 0)
		status = status % 256;
	return (not_so_fake_exit(shell, (unsigned char)(status)));
}
