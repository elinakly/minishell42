/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: Mika Schipper <mschippe@student.codam.n      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/04/28 14:03:31 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/04/28 16:02:03 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include "../../../include/path.h"

int	cd(t_shell shell, t_command *cmd)
{
	char	*homedir;
	int		res;

	if (!cmd)
		return (1);
	if (cmd->argc > 2)
		return (ft_putstr_fd("minishell: cd: too many arguments\n", 2), 1);
	if (cmd->argc == 1)
		homedir = get_homedir(shell);
	else
		homedir = expand_dir(shell, cmd->argv[1]);
	if (!homedir)
		return (1);
	res = chdir(homedir);
	if (res == 0)
		return (0);
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(homedir, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	free(homedir);
	return (1);
}
