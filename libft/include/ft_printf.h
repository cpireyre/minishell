/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 14:50:58 by copireyr          #+#    #+#             */
/*   Updated: 2024/05/27 11:49:16 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stddef.h>
# include <stdint.h>
# include "./ft_printf/parse_format.h"
# include "./ft_printf/format_bonus.h"
# include "./ft_printf/emit.h"

int				ft_isdigit(int c);
int				ft_atoi(const char *str);
size_t			ft_strlen(const char *s);
void			ft_bzero(void *s, size_t n);
char			*ft_strchr(const char *s, int c);
void			format_char(t_emitter *e, t_spec s, int c);
void			format_integer(t_emitter *e, t_spec s, int n);
void			format_lower_hexadecimal(t_emitter *e, t_spec s,
					uintmax_t n);
void			format_upper_hexadecimal(t_emitter *e, t_spec s,
					uintmax_t n);
void			format_string(t_emitter *e, t_spec s, char *str);
t_format_output	format_unsigned(t_spec s, unsigned long n, char *base);
void			format_pointer(t_emitter *e, t_spec s, uintptr_t ptr);
void			format_unsigned_integer(t_emitter *e, t_spec s, uintmax_t n);

#endif
