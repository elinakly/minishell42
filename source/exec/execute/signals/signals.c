/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 16:51:58 by eklymova          #+#    #+#             */
/*   Updated: 2025/05/21 06:17:34 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_handler(int sig)
{
	g_recv_sig = sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	signal_handler_child(int sig)
{
	g_recv_sig = sig;
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	set_main_signal(void)
{
	g_recv_sig = 0;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	set_child_signal(void)
{
	g_recv_sig = 0;
	signal(SIGINT, signal_handler_child);
	signal(SIGQUIT, SIG_IGN);
}
