/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 17:03:53 by mschippe          #+#    #+#             */
/*   Updated: 2025/05/24 17:04:33 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_recv_sig = 0;

t_shell	make_shell(char **envp)
{
	t_shell	shell;

	shell.cmds_count = 0;
	shell.heredoc_counter = 0;
	shell.last_parse_res = NONE;
	shell.status = 0;
	shell.main_rl_str = NULL;
	shell.pipes = NULL;
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

void	get_prompt(t_shell *shell)
{
	char	*prompt;

	if (isatty(fileno(stdin)))
		shell->main_rl_str = ft_readline(shell, shell->venv_arr);
	else
	{
		prompt = get_next_line(fileno(stdin));
		if (!prompt)
		{
			total_cleanup(shell);
			exit(shell->status);
		}
		shell->main_rl_str = ft_strtrim(prompt, "\n");
		free(prompt);
	}
}

void	finish_loop(t_shell *shell)
{
	add_history(shell->main_rl_str);
	history(shell, shell->main_rl_str);
	loop_cleanup(shell);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell		shell;

	shell = make_shell(envp);
	if (get_history(&shell))
		return (1);
	while (shell.loop_active)
	{
		set_main_signal();
		get_prompt(&shell);
		shell.last_parse_res = parse_commands(&shell, &shell.cmd);
		if (shell.last_parse_res == PARSEOK && exec_heredocs(&shell, shell.cmd))
			shell.status = execute_cmds(&shell, shell.cmd, shell.venv_arr);
		else if (shell.last_parse_res == SYNTAX_ERROR)
		{
			shell.status = 2 + argv + argc - argc - argv;
			write(2, "minishell: syntax error\n", 24);
		}
		finish_loop(&shell);
		shell.venv_arr = venv_to_arr(shell.venv);
		if (!shell.venv_arr)
			break ;
	}
	end_cleanup(&shell);
	return (shell.status);
}
