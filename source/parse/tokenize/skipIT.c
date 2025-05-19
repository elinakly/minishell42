
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/variable.h"
#include "../include/tokenize.h"
#include "../lib/libft/libft.h"

int		skip_quoted(char *str);
bool	is_escaped_char(char *str, size_t index);

/**
 * Calculates the length of an environment variable token
 * @param str The string in which to look (can be offset)
 * @returns The amount of characters to move forward in order to skip the token
 */
size_t	skip_env_var(char *str)
{
	size_t	count;

	if (!str || *str != MC_VARIABLE)
		return (0);
	count = 1;
	while (is_env_var_char(str[count]))
		count++;
	return (count);
}

size_t	skip_heredoc(char *str)
{
	e_metachar	quot;
	size_t		count;

	quot = MC_NONE;
	count = 0;
	while (str[count])
	{
		if (set_quote_state(str, count, &quot))
			count++;
		else if (quot == MC_NONE && (ft_isalnum(str[count])
				|| ((unsigned char)(str[count]) >= 128)
			|| ft_strchr(HEREDOC_SPECIAL_CHARS, str[count])))
			count++;
		else if (quot == MC_DQUOTE || quot == MC_SQUOTE)
			count++;
		else if (is_escaped_char(str, count) || str[count] == '\\')
			count++;
		else
			break ;
	}
	return (count);
}

/**
 * Calculates the length of an input redirection token
 * 
 * Works for both a regular redirection and heredoc (basic, no quote support)
 * @param str The string in which to look (can be offset)
 * @returns The amount of characters to move forward in order to skip the token
 */
size_t	skip_redir_in(char *str)
{
	if (!str || *str != MC_REDIR_IN)
		return (0);
	if (*(str + 1) != MC_REDIR_IN)
		return (1);
	return (2);
}

/**
 * Calculates the length of any supported meta character token
 * @param str The string in which to look (can be offset)
 * @returns The amount of characters to move forward in order to skip the token
 */
size_t	skip_meta(char *str)
{
	if (!str)
		return (0);
	if (*str == MC_PIPE || *str == MC_SEPAR_SPACE || *str == MC_SEPAR_TAB)
		return (1);
	if (*str == MC_ESCAPE)
		return (2);
	if (*str == MC_DQUOTE || *str == MC_SQUOTE)
		return (skip_quoted(str));
	if (*str == MC_REDIR_OUT)
		return (1 + (*(str + 1) == MC_REDIR_OUT));
	if (*str == MC_VARIABLE)
		return (skip_env_var(str));
	if (*str == MC_REDIR_IN)
		return (skip_redir_in(str));
	return (0);
}

/**
 * Skips any spaces until finding another character or end of string
 * Does this by increasing the char pointer you point to
 * @param str The string you want to skip spaces on
 * @returns The number of spaces it skipped
 */
size_t	skip_spaces(char *str)
{
	size_t	spaces;

	spaces = 0;
	while (str && (str[spaces] == ' ' || str[spaces] == '\t'
			|| str[spaces] == '\n'))
		spaces++;
	return (spaces);
}
