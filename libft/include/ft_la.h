/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_la.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 09:55:11 by copireyr          #+#    #+#             */
/*   Updated: 2024/07/05 12:36:16 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LA_H
# define FT_LA_H

typedef struct s_v4i
{
	int	x;
	int	y;
	int	z;
	int	c;
}		t_v4i;

typedef struct s_v3d
{
	double	x;
	double	y;
	double	z;
}				t_v3d;

typedef struct s_v2i
{
	int	x;
	int	y;
}				t_v2i;

typedef struct s_v3i
{
	int	x;
	int	y;
	int	z;
}				t_v3i;

double	ft_distance(int x0, int y0, int x1, int y1);

int		ft_abs(int a);
int		ft_min(int a, int b);
int		ft_max(int a, int b);
int		ft_sign(int a, int b);

t_v2i	v2i(int x, int y);
t_v2i	v2i3d(t_v3d a);
t_v2i	v2i_sub(t_v2i a, t_v2i b);
t_v2i	v2i_mul(t_v2i a, t_v2i b);
t_v2i	v2i_div(t_v2i a, t_v2i b);
t_v2i	v2i_add(t_v2i a, t_v2i b);
int		v2i_eq(t_v2i a, t_v2i b);
int		v2i_sqrlen(t_v2i a);

t_v3i	v3i_add(t_v3i a, t_v3i b);
t_v3i	v3i(int x, int y, int z);
t_v3i	v3i_mul(t_v3i a, t_v3i b);
t_v3i	v3ii(int a);

t_v3d	v3d_add(t_v3d a, t_v3d b);
t_v3d	v3dd(double x);
t_v3d	v3d_div(t_v3d a, t_v3d b);
t_v3d	v3d_mul(t_v3d a, t_v3d b);

#endif /* FT_LA_H */
