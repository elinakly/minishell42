/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:50:45 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/03/18 15:54:23 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATE_H
# define VALIDATE_H

#include "tokenandvar.h"
#include "structbuild.h"
#include "tokenize.h"

e_parse_result	validate_tokens(t_token *token);
e_parse_result	validate_cmd_str(char *cmd);
#endif