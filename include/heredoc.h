/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:26:03 by mschippe          #+#    #+#             */
/*   Updated: 2025/05/08 19:12:46 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

char	*get_heredoc(char *delim, bool expand);
char	*heredoc(t_shell *shell, char *delim, bool expand);
bool	exec_heredocs(t_shell *shell, t_command *cmd);
#endif