/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:24:30 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/19 21:37:44 by mika             ###   ########.fr       */
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
#include "../include/signals.h"

char	*ft_readline(t_shell *shell, char **envp);

t_shell	make_shell(char **envp)
{
	t_shell	shell;

	shell.cmds_count = 0;
	shell.heredoc_counter = 0;
	shell.last_parse_res = NONE;
	shell.status = 0;
	shell.main_rl_str = NULL;
	shell.cmd = NULL;
	shell.loop_active = false;
	shell.venv = make_venv(envp);
	if (!shell.venv)
		return (shell);
	shell.venv_arr = venv_to_arr(shell.venv);
	if (!shell.venv_arr)
		return (free_venv(shell.venv), shell);
	shell.loop_active = true;
	return (shell);
}

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

int	main(int argc, char **argv, char **envp)
{
	t_command		*cmds;
	t_shell			shell;

	if (get_history())
		return (1);
	shell = make_shell(envp);
	while (shell.loop_active)
	{
		set_main_signal();
		if (isatty(fileno(stdin)))
			shell.main_rl_str = ft_readline(&shell, shell.venv_arr);
		else
		{
			char *prompt = get_next_line(fileno(stdin));
			if (!prompt)
				break ;
			shell.main_rl_str = ft_strtrim(prompt, "\n");
			free(prompt);
		}
		if (!shell.main_rl_str)
			break ;
		if (!shell.main_rl_str)
			return (1);
		shell.last_parse_res = parse_commands(&shell, &shell.cmd);
		if (shell.last_parse_res == PARSEOK && exec_heredocs(&shell, shell.cmd))
			shell.status = execute_cmds(&shell, shell.cmd, shell.venv_arr);
		else if (shell.last_parse_res == SYNTAX_ERROR)
		{
			shell.status = 2;
			write(2, "minishell: syntax error\n", 24);
		}
		add_history(shell.main_rl_str);
		history(shell.main_rl_str);
		loop_cleanup(&shell);
		shell.venv_arr = venv_to_arr(shell.venv);
		if (!shell.venv_arr)
			break ;
	}
	end_cleanup(&shell);
	return (shell.status);
}
