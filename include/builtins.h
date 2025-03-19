/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 16:35:59 by eklymova          #+#    #+#             */
/*   Updated: 2025/03/19 14:34:42 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <unistd.h>
# include "../lib/libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <linux/limits.h>
# include <fcntl.h>

int		echo(char **args);
int		env(char **envp);
int		is_builtin(char *cmd, char **args, char **envp);
int		pwd(void);
int		ft_exit(void);
int		history(char *line);

#endif