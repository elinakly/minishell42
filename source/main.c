/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: eklymova <eklymova@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/06 15:24:30 by eklymova      #+#    #+#                 */
/*   Updated: 2025/03/12 04:34:04 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include "../include/variable.h"
#include "../include/tokenize.h"
#include "../include/memory.h"
#include "../include/builtins.h"

char			*ft_readline(char **envp);
int				skip_spaces(char **str);
int				skip_quoted(char **str);
bool			has_unclosed_quote(char *str, e_quote_type *type);
size_t			count_tokens(char *entry);
char			**tokenize(char *entry, size_t *tokencount);
e_token_type	get_token_type(char *raw_token, e_token_type last, bool *cmdfound);
size_t			get_var_count(char *cmd);
t_part_var		**get_var_names(char *cmd, size_t varcount, t_part_var **names);
int				is_builtin(char *command);
t_env_var		**get_command_vars(t_part_var **names);
size_t			calc_expanded_len(char *cmd, t_env_var **vars);
char			*get_expanded_cmd(char *cmd, t_env_var **vars,
									size_t resindex, size_t cmdindex);
char			*sanitize_token(char *token);

const char *token_type_to_string(e_token_type type)
{
    switch (type)
    {
        case TT_UNKNOWN: return "TT_UNKNOWN";
        case TT_COMMAND: return "TT_COMMAND";
        case TT_ARGUMENT: return "TT_ARGUMENT";
        case TT_HEREDOC: return "TT_HEREDOC";
        case TT_PIPE: return "TT_PIPE";
        case TT_RE_IN: return "TT_RE_IN";
        case TT_RE_OUT: return "TT_RE_OUT";
        case TT_RE_OUT_APPEND: return "TT_RE_OUT_APPEND";
		case TT_INFILE: return "TT_INFILE";
		case TT_OUTFILE: return "TT_OUTFILE";
        default: return "INVALID_TYPE";
    }
}

void test_parse_output(char *test, char** envp)
{
	int tokenindex = 0;
	size_t amount = 0;
	size_t varindex = 0;
	e_token_type lasttype = TT_UNKNOWN;
	bool cmdfound = false;

	size_t varcount = get_var_count(test);
	t_part_var **varnames = malloc(sizeof(t_part_var *) * (varcount + 1));
	if (!varnames)
		return; // ehh handle better later
	get_var_names(test, varcount, varnames);
	varnames[varcount] = NULL;
	t_env_var **variables = get_command_vars(varnames);
	if (!variables)
		return;
	char *expanded = get_expanded_cmd(test, variables, 0, 0);
	char **tokens = tokenize(expanded, &amount);
	tokenindex = 0;
	while (tokens[tokenindex])
	{
		e_token_type tokentype = get_token_type(tokens[tokenindex], lasttype, &cmdfound);
		lasttype = tokentype;
		tokens[tokenindex] = sanitize_token(tokens[tokenindex]);
		printf("%-16s [%s]\n", token_type_to_string(tokentype), tokens[tokenindex]);
		tokenindex++;
	}
	free(expanded);
	free_array((void **)tokens, NULL);
	free_array((void **)varnames, &clear_part_var);
	free_array((void **)variables, &clear_env_var);
}

int	main(int argc, char **argv, char **envp)
{
	char *test;
	while (1)
	{
		if (!(test = ft_readline(envp)))
			return (1);
		// int builtin = is_builtin(test);
		// Just made this function to keep main() a bit readable
		test_parse_output(test, envp);
		free(test);
	}
	return (0);
}