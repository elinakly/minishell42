/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   path.h                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: Mika Schipper <mschippe@student.codam.n      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/04/05 22:39:39 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/04/05 22:51:59 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

typedef struct t_path t_path;

struct t_path
{
	char	*name;
	t_path	*prev;
	t_path	*next;
};