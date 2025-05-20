/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 16:33:11 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/20 18:02:53 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset(t_shell *shell, t_command *cmds, char **envp)
{
	int	i;

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
