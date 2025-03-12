/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   memory.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: Mika Schipper <mschippe@student.codam.n      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/02/20 01:24:49 by Mika Schipp   #+#    #+#                 */
/*   Updated: 2025/03/12 04:23:31 by Mika Schipp   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
# define MEMORY_H
void	free_array(void **arr, void (*del)(void *));
void	clear_env_var(void *var);
void	clear_part_var(void *var);
size_t	ft_arrlen(void **arr);
#endif