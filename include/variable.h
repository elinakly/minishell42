/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   variable.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mschippe <mschippe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/09 13:49:41 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/03/13 02:59:08 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef VARIABLE_H
# define VARIABLE_H
# include <stdbool.h>
# include <stdlib.h>
#include "tokenandvar.h"

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

bool		set_quote_state(char *cmd, size_t index, e_metachar *current);
bool		is_var_char(char c, bool is_first);
size_t		skip_var_chars(char *cmd, size_t index);
size_t		get_var_count(char *cmd);
size_t		var_len(char *cmd, size_t i);
t_part_var	**get_var_names(char *cmd, size_t varcount, t_part_var **names);
t_env_var	*make_var(t_part_var *part);
t_env_var	**get_command_vars(t_part_var **names);
size_t		calc_expanded_len(char *cmd, t_env_var **vars);
char		*get_escaped_value(t_env_var *var);
bool		insert_var(char *res, t_env_var *var, size_t *index);
char		*get_expanded_cmd(char *cmd, t_env_var **vars);
#endif
