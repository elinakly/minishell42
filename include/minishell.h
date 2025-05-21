/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:45:44 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/05/21 06:17:03 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <errno.h>
# include <stdio.h>
# include <fcntl.h>
# include <limits.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include "../lib/libft/libft.h"
# include "enums.h"
# include "structs.h"
# include "builtins.h"
# include "execute.h"
# include "heredoc.h"
# include "memory.h"
# include "path.h"
# include "pipex_bonus.h"
# include "signals.h"
# include "structbuild.h"
# include "tokenize.h"
# include "validate.h"
# include "variable.h"
# include "venv.h"

extern int	g_recv_sig;
#endif
