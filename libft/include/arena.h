/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 10:04:53 by copireyr          #+#    #+#             */
/*   Updated: 2024/05/28 11:14:58 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARENA_H
# define ARENA_H

typedef struct s_arena	*t_arena;

struct s_arena
{
	t_arena	prev;
	char	*avail;
	char	*limit;
};

union u_align
{
	int			i;
	long		l;
	long		*lp;
	void		*p;
	void		(*fp)(void);
	float		f;
	double		d;
	long double	ld;
};

union u_header
{
	struct s_arena	b;
	union u_align	a;
};

extern t_arena	arena_new(void);
extern void		arena_dispose(t_arena *ap);
extern void		*arena_alloc(t_arena arena, size_t nbytes);
extern void		*arena_calloc(t_arena arena, size_t count, size_t nbytes);
extern void		arena_free(t_arena arena);

#endif /* ARENA_H */
