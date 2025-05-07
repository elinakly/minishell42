/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   venv.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:00:32 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/05/07 10:49:13 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VENV_H
# define VENV_H
# include "minishell.h"

t_venv	*make_venv(char **envp);
char	*get_env_val(t_shell *shell, char *name);
char	**venv_to_arr(t_venv *base);
bool	add_env_var(t_venv *base, char *strvar);
void	remove_env_var(t_venv **base, char *name);
bool	simple_add_var(t_venv *base, char *name, char *value);
#endif