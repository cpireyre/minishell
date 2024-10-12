/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 20:08:57 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/12 16:05:01 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLOB_H
# define GLOB_H

char	**dir_entries(t_arena arena, const char *filepath);
void	glob(t_arena arena, t_ast_node *ast);

#endif /* GLOB_H */
