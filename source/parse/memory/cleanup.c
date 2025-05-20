/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:48:58 by mschippe          #+#    #+#             */
/*   Updated: 2025/05/20 18:49:13 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	loop_cleanup(t_shell *shell)
{
	if (shell->cmd)
		free_commands(shell->cmd);
	free(shell->main_rl_str);
	free_array((void **)shell->venv_arr, NULL);
	shell->venv_arr = NULL;
	shell->main_rl_str = NULL;
	shell->cmd = NULL;
}

void	end_cleanup(t_shell *shell)
{
	rl_clear_history();
	free_venv(shell->venv);
	free_array((void **)shell->venv_arr, NULL);
	shell->venv_arr = NULL;
	shell->venv = NULL;
}

void	total_cleanup(t_shell *shell)
{
	loop_cleanup(shell);
	end_cleanup(shell);
}
