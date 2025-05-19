/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:06:03 by mika              #+#    #+#             */
/*   Updated: 2025/05/18 15:07:24 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

void	set_main_signal(void);
void	signal_handler(int sig);
void	signal_handler_child(int sig);
void	signal_handler_heredoc(int sig);
void	set_ignore_signal(void);
void	set_child_default_signal(void);
void	set_child_signal(void);
void	set_heredoc_signal(void);
#endif