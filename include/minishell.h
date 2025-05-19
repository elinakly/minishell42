/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:45:44 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/05/19 14:45:08 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "enums.h"
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

extern int				g_recv_sig;

typedef struct t_venv	t_venv;


struct t_venv
{
	bool	base;
	char	*name;
	char	*value;
	t_venv	*next;
};

typedef struct t_part_var
{
	char		*name;
	e_metachar	in_quote_type;
}				t_part_var;

typedef struct t_env_var
{
	char		*name;
	char		*value;
	e_metachar	quote_type;
}				t_env_var;

typedef struct t_triple_index
{
	size_t	var;
	size_t	cmd;
	size_t	res;
}			t_triple_index;

typedef struct t_tokeninfo
{
	e_token_type	lasttype;
	bool			cmdfound;
	size_t			index;
}					t_tokeninfo;

typedef struct t_token	t_token;

struct t_token
{
	e_token_type	type;
	char			*raw_value;
	char			*value;
	t_token			*next;
};

typedef struct t_redirect	t_redirect;

struct t_redirect
{
	e_redir_type	type;
	char			*file;
	int				in_fd;
	int				out_fd;
	char			*heredoc_delim;
	bool			expand_in_heredoc;
	t_redirect		*next;
};


typedef struct t_command	t_command;
/**
 * Contains all information about a command
 * name: The command name
 * argc: The amount of arguments
 * argv: The arguments, processed and split properly into an array
 * has_pipe: Whether the output should be redirected to the next command
 * has_redirects: Whether input/output should be redirected
 * redirects: a linkedlist with redirect type and file path
 * next: the next command in this linkedlist of commands
 */
struct t_command
{
	bool			has_command;
	bool			has_redirects;
	char			*name;
	size_t			argc;
	char			**argv;
	pid_t			pid;
	t_redirect		*redirects;
	t_command		*next;
};

typedef struct s_shell
{
	char			*main_rl_str;
	e_parse_result	last_parse_res;
	int				last_status;
	t_venv			*venv;
	bool			loop_active;
	int				heredoc_counter;
	int				cmds_count;
}	t_shell;
#endif
