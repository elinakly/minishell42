/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 13:50:36 by eklymova          #+#    #+#             */
/*   Updated: 2025/03/06 19:10:36 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../lib/libft/libft.h"
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