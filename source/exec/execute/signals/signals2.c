/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:11:05 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/21 06:17:43 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_ignore_signal(void)
{
	g_recv_sig = 0;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	set_child_default_signal(void)
{
	g_recv_sig = 0;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	signal_handler_heredoc(int sig)
{
	char	newline;

	newline = '.';
	printf("\n");
	g_recv_sig = sig;
	rl_done = 1;
	ioctl(STDIN_FILENO, TIOCSTI, &newline);
}

void	set_heredoc_signal(void)
{
	signal(SIGINT, signal_handler_heredoc);
	signal(SIGQUIT, SIG_IGN);
}
