/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mika <mika@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 18:14:49 by mschippe          #+#    #+#             */
/*   Updated: 2025/05/21 06:11:11 by mika             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct s_venv		t_venv;
typedef struct s_token		t_token;
typedef struct s_redirect	t_redirect;
typedef struct s_command	t_command;

struct s_venv
{
	bool	base;
	char	*name;
	char	*value;
	t_venv	*next;
};

typedef struct t_part_var
{
	char		*name;
	t_metachar	in_quote_type;
}				t_part_var;

typedef struct t_env_var
{
	char		*name;
	char		*value;
	t_metachar	quote_type;
}				t_env_var;

typedef struct t_triple_index
{
	size_t	var;
	size_t	cmd;
	size_t	res;
}			t_triple_index;

typedef struct t_tokeninfo
{
	t_token_type	lasttype;
	bool			cmdfound;
	size_t			index;
}					t_tokeninfo;

struct s_token
{
	t_token_type	type;
	char			*raw_value;
	char			*value;
	t_token			*next;
};

struct s_redirect
{
	t_redir_type	type;
	char			*file;
	int				in_fd;
	int				out_fd;
	char			*heredoc_delim;
	bool			expand_in_heredoc;
	t_redirect		*next;
};

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
struct s_command
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
	t_parse_result	last_parse_res;
	int				status;
	t_venv			*venv;
	char			**venv_arr;
	t_command		*cmd;
	bool			loop_active;
	int				heredoc_counter;
	int				cmds_count;
	int				**pipes;
}	t_shell;
#endif
