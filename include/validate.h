/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   validate.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: mschippe <mschippe@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/16 16:50:45 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/04/16 12:42:59 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATE_H
# define VALIDATE_H
# include "minishell.h"

e_parse_result	validate_tokens(t_token *token);
e_parse_result	validate_cmd_str(char *cmd);
#endif