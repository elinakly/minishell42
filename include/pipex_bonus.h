/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:50:36 by eklymova          #+#    #+#             */
/*   Updated: 2025/02/20 20:10:06 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "ft_printf.h"
# include "libft.h"
# include <unistd.h>
# include <stdio.h>
# include <sys/wait.h>
# include <limits.h>
# include <fcntl.h>
# include <errno.h>
# include <linux/limits.h>

typedef struct s_command
{
	char	*cmd;
	int		input_fd;
	int		output_fd;
	int		argc;
	char	**args;
	int		num_cmds;
}			t_command;

void		execute(char *com, char **envp);
int			error(int status);
void		create_pipes(int num_cmds, int **pipes);
void		free_arr(char **arr);

#endif