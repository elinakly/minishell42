/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:26:03 by mschippe          #+#    #+#             */
/*   Updated: 2025/05/24 16:43:36 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

char	*get_heredoc(t_shell *shell, char *delim, bool expand, char *line);
char	*heredoc(t_shell *shell, char *delim, bool expand);
bool	exec_heredocs(t_shell *shell, t_command *cmd);
bool	check_heredoc_cancel(char *prompt);
char	*heredoc_prompt(void);
char	*generate_filename(t_shell *shell, char *delim);
char	*ft_strjoin_nl(const char *s1, const char *s2);
#endif