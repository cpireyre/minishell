/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   emit.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:51:19 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/01 11:44:02 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EMIT_H
# define EMIT_H

# include <sys/types.h>
# include <unistd.h>
# include "ft_printf/format_bonus.h"

# define EMITTER_BUFFER_SIZE 256

enum e_mode
{
	FT_SNPRINTF,
	FT_DPRINTF,
};

typedef struct s_emitter
{
	char		buf[EMITTER_BUFFER_SIZE];
	size_t		idx;
	ssize_t		written;
	int			fd;
	char		*output_string;
	size_t		output_string_size;
	enum e_mode	mode;
}				t_emitter;

void	emit_char(t_emitter *e, char c);
void	emit_string(t_emitter *e, char *s);
void	emit_buf(t_emitter *e, char *buf, size_t len);
void	emit_int(t_emitter *e, t_format_output *o);
void	emit_flush(t_emitter *e);

#endif
