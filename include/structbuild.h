/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structbuild.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 00:17:13 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/03/19 13:11:12 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTBUILD_H
# define STRUCTBUILD_H
# include "tokenize.h"

typedef struct t_tokeninfo
{
	e_token_type	lasttype;
	bool			cmdfound;
	size_t			index;
}					t_tokeninfo;

typedef struct t_token t_token;

struct t_token
{
	e_token_type	type;
	char			*raw_value;
	char			*value;
	t_token			*next;
};

typedef struct t_redirect t_redirect;

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


typedef struct t_command t_command;
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

typedef enum e_parse_result
{
	UNCLOSED_SQUOTE,
	UNCLOSED_DQUOTE,
	UNFINISHED_PIPE,
	ESCAPED_NEWLINE,
	HEREDOC,
	SYNTAX_ERROR,
	MALLOC_FAIL,
	EMPTY,
	PARSEOK,
}	e_parse_result;

t_env_var		**get_vars_from_cmd(char *cmd);
t_token			*get_tokens_from_cmd(char *cmd, t_env_var **vars, size_t *tokencount);
t_command		*make_cmd_list(t_token *token);
e_parse_result	parse_commands(char *cmdstr, t_command **cmd);
#endif