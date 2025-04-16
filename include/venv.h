/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   venv.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: Mika Schipper <mschippe@student.codam.n      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/04/14 17:00:32 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/04/16 12:42:12 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef VENV_H
# define VENV_H
# include "minishell.h"

t_venv	*make_venv(char **envp);
char	*get_env(t_venv *envp, char *name);
#endif