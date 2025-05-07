/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:24:30 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/07 10:56:34 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/tokenize.h"
#include "../include/variable.h"
#include "../include/path.h"
#include "../include/memory.h"
#include "../include/builtins.h"
#include "../include/execute.h"
#include "../include/structbuild.h"
#include "../include/validate.h"
#include "../include/minishell.h"
#include "../include/venv.h"
#include "../include/heredoc.h"

char			*ft_readline(t_shell *shell, char **envp);
size_t			ft_cmdcount(t_command *head);

int	main(int argc, char **argv, char **envp)
{
	t_command		*cmds;
	t_shell			shell;

	set_signal();
	if (get_history())
		return (1);
	//TODO: Check cwd and venv NULL in shell
	//TODO: Free everything inside shell, some even still need functions made
	shell = (t_shell){NULL, NONE, 0, make_venv(envp), true};
	while (shell.loop_active)
	{
		shell.main_rl_str = ft_readline(&shell, venv_to_arr(shell.venv)); //TODO: venv array leaks
		if (!shell.main_rl_str)
			break ;
		if (ft_strncmp(shell.main_rl_str, "heredoc", 9) == 0)
		{
			printf("%s\n", get_heredoc("test", true));
			continue ;
		}
		if (!shell.main_rl_str)
			return (1);
		shell.last_parse_res = parse_commands(&shell, &cmds);
		if (shell.last_parse_res == PARSEOK)
		{
			shell.last_status = execute_cmds(&shell, cmds, venv_to_arr(shell.venv), ft_cmdcount(cmds)); //TODO: venv array leaks
			free_commands(cmds);
		}
		else if (shell.last_parse_res != EMPTY)
			shell.last_status = 2;
		add_history(shell.main_rl_str);
		history(shell.main_rl_str);
		free(shell.main_rl_str);
	}
	// TODO: We can't exit the loop so this is probably never actually reached, we will need to handle it in our exit functions
	rl_clear_history();
	free_venv(shell.venv);
	return (shell.last_status); // probably? maybe needs to be different depending on signal or whatever
}
