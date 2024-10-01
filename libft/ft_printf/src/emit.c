/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   emit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:29:56 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/01 12:07:28 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf/emit.h"
#include "libft.h"

void	emit_char(t_emitter *e, char c)
{
	if (e->written != -1)
	{
		e->buf[e->idx] = c;
		e->idx++;
		if (e->idx == EMITTER_BUFFER_SIZE)
			emit_flush(e);
	}
}

void	emit_string(t_emitter *e, char *s)
{
	while (*s)
		emit_char(e, *s++);
}

void	emit_buf(t_emitter *e, char *buf, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
		emit_char(e, buf[i++]);
}

void	emit_int(t_emitter *e, t_format_output *o)
{
	size_t	i;

	i = 0;
	if (o->pad_character == ' ')
		while (i++ < o->left_padding)
			emit_char(e, o->pad_character);
	if (o->sign)
		emit_char(e, o->sign);
	if (o->prefix[0])
		emit_buf(e, o->prefix, 2);
	if (o->pad_character == '0')
		while (i++ < o->left_padding)
			emit_char(e, o->pad_character);
	i = 0;
	while (i++ < o->middle_padding)
		emit_char(e, '0');
	emit_string(e, o->buf);
	i = 0;
	while (i++ < o->right_padding)
		emit_char(e, o->pad_character);
}

void	emit_flush(t_emitter *e)
{
	ssize_t			ret;
	const size_t	can_copy = ft_size_t_min(e->idx, e->output_string_size);

	if (e->mode == FT_SNPRINTF)
	{
		if (e->output_string)
			ft_memcpy(e->output_string + e->written, e->buf, can_copy);
		e->output_string_size -= can_copy;
		e->written += can_copy;
		e->idx = 0;
	}
	else if (e->idx != 0)
	{
		ret = write(e->fd, e->buf, e->idx);
		if (ret == -1)
			e->written = -1;
		else
		{
			e->written += ret;
			e->idx = 0;
		}
	}
}
