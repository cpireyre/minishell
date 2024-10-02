/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vsnprintf.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:02:53 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/01 12:47:33 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdlib.h>

void				convert(t_emitter *e, t_spec s, va_list *ap);
static t_emitter	init_emitter(char *str, size_t size);

int	ft_vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
	t_emitter	e;
	va_list		copy;

	if (!size)
		return (-1);
	e = init_emitter(str, size);
	va_copy(copy, ap);
	if (str)
		ft_bzero(str, size--);
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
		format += *format != 0;
	}
	if (e.idx && e.written != -1)
		emit_flush(&e);
	va_end(copy);
	return ((int)e.written);
}

static t_emitter	init_emitter(char *str, size_t size)
{
	t_emitter	e;

	ft_bzero(&e, sizeof(e));
	e.output_string = str;
	e.output_string_size = size;
	e.mode = FT_SNPRINTF;
	return (e);
}

void	convert(t_emitter *e, t_spec s, va_list *ap)
{
	if (s.conversion == UNSIGNED_CHAR)
		format_char(e, s, va_arg(*ap, int));
	else if (s.conversion == STRING)
		format_string(e, s, va_arg(*ap, char *));
	else if (s.conversion == POINTER)
		format_pointer(e, s, va_arg(*ap, uintptr_t));
	else if (s.conversion == INTEGER)
		format_integer(e, s, va_arg(*ap, int));
	else if (s.conversion == UNSIGNED_INTEGER)
		format_unsigned_integer(e, s, va_arg(*ap, uintmax_t));
	else if (s.conversion == LOWER_HEXADECIMAL)
		format_lower_hexadecimal(e, s, va_arg(*ap, unsigned int));
	else if (s.conversion == UPPER_HEXADECIMAL)
		format_upper_hexadecimal(e, s, va_arg(*ap, unsigned int));
	else if (s.conversion == PERCENT)
		format_char(e, s, '%');
}

int	ft_snprintf(char *str, size_t size, const char *format, ...)
{
	va_list	ap;
	int		ret;

	va_start(ap, format);
	ret = ft_vsnprintf(str, size, format, ap);
	va_end(ap);
	return (ret);
}

char	*ft_allocaprintf(const char *format, ...)
{
	va_list	ap;
	va_list	copy;
	size_t	size;
	char	*ret;

	va_start(ap, format);
	va_copy(copy, ap);
	size = ft_vsnprintf(NULL, SIZE_MAX, format, ap) + 1;
	va_end(ap);
	ret = malloc(size);
	if (ret)
		ft_vsnprintf(ret, size, format, copy);
	va_end(copy);
	return (ret);
}
