/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:03:31 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/05/20 18:01:43 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_cd_err(char *homedir, char *cwd)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(homedir, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	free(homedir);
	free(cwd);
	return (1);
}

int	cd(t_shell *shell, t_command *cmd)
{
	char	*home;
	char	*path;
	char	*cwd;
	int		res;

	path = ft_strdup("PWD");
	if (!cmd || !path)
		return (free(path), 1);
	if (cmd->argc > 2 && !(cmd->argc == 3 || strequals(cmd->argv[1], "--")))
		return (free(path), ft_putstr_fd(CD_TM_ARGS, 2), 1);
	if (cmd->argc == 1 || (cmd->argc == 2 && strequals(cmd->argv[1], "--")))
		home = get_homedir(shell);
	else
		home = expand_dir(shell, cmd->argv[1 + strequals(cmd->argv[1], "--")]);
	if (!home)
		return (free(path), 1);
	res = chdir(home);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (free(home), free(path), res);
	if (res == 0)
		return (simple_add_var(shell->venv, path, cwd),
			free(cwd), free(home), free(path), 0);
	free(path);
	return (print_cd_err(home, cwd));
}
