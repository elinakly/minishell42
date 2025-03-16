/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   validate.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: Mika Schipper <mschippe@student.codam.n      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/16 16:50:45 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/03/16 17:25:29 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATE_H
# define VALIDATE_H

#include "tokenandvar.h"
#include "structbuild.h"
#include "tokenize.h"

e_parse_result	validate_tokens(t_token **tokens);
e_parse_result	validate_cmd_str(char *cmd);
#endif