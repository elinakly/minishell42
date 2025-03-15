/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eklymova <eklymova@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:24:30 by eklymova          #+#    #+#             */
/*   Updated: 2025/03/13 18:32:10 by eklymova         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdbool.h>
#include "../include/tokenize.h"
#include "../include/variable.h"
#include "../include/memory.h"
#include "../include/builtins.h"
#include "../include/execute.h"
#include "../include/structbuild.h"
#include <readline/history.h>

char			*ft_readline(char **envp);

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

void test_parse_output(char *test, bool isdebug)
{
	t_env_var **variables = get_vars_from_cmd(test);
	if (!variables)
		return;
	t_token **tokens = get_tokens_from_cmd(test, variables);
	free_array((void **)variables, &clear_env_var);
	if (!tokens)
		return;
	size_t tokenindex = 0;
	while (isdebug && tokens[tokenindex])
		printf("%-16s [%s]\n", token_type_to_string(tokens[tokenindex]->type), tokens[tokenindex++]->value);
	free_array((void **)tokens, &clear_token_var);
}
void test_execute(char *test, char **envp)
{
	char **args = ft_split(test, ' ');
	int builtin = is_builtin(test, args, envp);
}

int	main(int argc, char **argv, char **envp)
{
	char *test;
	set_signal();	
	while (1)
	{
		if (!(test = ft_readline(envp)))
			return (1);
		test_execute(test, envp);
		// Just made this function to keep main() a bit readable
		test_parse_output(test, argc == 2 && !ft_strncmp(argv[1], "--debug", 255));
		add_history(test);
		free(test);
	}
	rl_clear_history();
	return (0);
}