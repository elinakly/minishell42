/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschippe <mschippe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:50:45 by Mika Schipp       #+#    #+#             */
/*   Updated: 2025/05/20 18:16:08 by mschippe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATE_H
# define VALIDATE_H
# include "minishell.h"

t_parse_result	validate_tokens(t_token *token);
t_parse_result	validate_cmd_str(char *cmd);
#endif