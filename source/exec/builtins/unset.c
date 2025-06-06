/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:33:11 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/24 16:38:22 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset(t_shell *shell, t_command *cmds)
{
	size_t	i;

	i = 1;
	if (cmds->argc == 1)
		return (0);
	if (cmds->argv[1][0] == '-' && cmds->argv[1][1])
		return (ft_putstr_fd("unset: invalid option\n", 2), 2);
	while (i < cmds->argc)
	{
		if (cmds->argv[i][0] != '\0')
			remove_env_var(&shell->venv, cmds->argv[i]);
		i++;
	}
	return (0);
}
