/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 12:42:20 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/01 12:04:31 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	convert(t_emitter *e, t_spec s, va_list *ap);

static int	ft_vdprintf(int fd, const char *format, va_list ap)
{
	t_emitter	e;
	va_list		copy;

	va_copy(copy, ap);
	ft_bzero(&e, sizeof(e));
	e.fd = fd;
	e.mode = FT_DPRINTF;
	while (*format)
	{
		if (*format == '%')
		{
			convert(&e, parse_format(++format), &copy);
			while (*format && !ft_strchr(CONVERSIONS, *format))
				format++;
		}
		else
			emit_char(&e, *format);
		if (*format)
			format++;
	}
	if (e.idx && e.written != -1)
		emit_flush(&e);
	va_end(copy);
	return ((int)e.written);
}

int	ft_dprintf(int fd, const char *format, ...)
{
	va_list	ap;
	int		ret;

	va_start(ap, format);
	ret = ft_vdprintf(fd, format, ap);
	va_end(ap);
	return (ret);
}

int	ft_printf(const char *format, ...)
{
	va_list	ap;
	int		ret;

	va_start(ap, format);
	ret = ft_vdprintf(1, format, ap);
	va_end(ap);
	return (ret);
}
