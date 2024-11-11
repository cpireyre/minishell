/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 20:02:19 by copireyr          #+#    #+#             */
/*   Updated: 2024/11/10 22:03:42 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <stdbool.h>
#include "libft.h"
#include "expand.h"
#include "glob.h"
#include "ast.h"

/* static bool			match(const t_quote *pattern, const char *candidate); */
/* static const char	**get_cwd_entries(t_arena arena); */




/* static bool	match(const t_quote *pattern, const char *candidate) */
/* { */
/* 	if (!*pattern) */
/* 		return (!*candidate); */
/* 	if ((*pattern & CHAR_MASK) == '*' && !is_quoted(*pattern)) */
/* 	{ */
/* 		while ((*pattern & CHAR_MASK) == '*' && !is_quoted(*pattern)) */
/* 			pattern++; */
/* 		if (!*pattern) */
/* 			return (true); */
/* 		while (*candidate) */
/* 			if (match(pattern, candidate++)) */
/* 				return (true); */
/* 		return (false); */
/* 	} */
/* 	if ((*pattern & CHAR_MASK) == *candidate) */
/* 		return (match(++pattern, ++candidate)); */
/* 	return (false); */
/* } */

/* static const char	**get_cwd_entries(t_arena arena) */
/* { */
/* 	DIR				*dir; */
/* 	struct dirent	*entry; */
/* 	int				count; */
/* 	const char		**result; */

/* 	count = 0; */
/* 	dir = opendir("."); */
/* 	if (!dir) */
/* 		return (NULL); */
/* 	while (readdir(dir)) */
/* 		count++; */
/* 	closedir(dir); */
/* 	result = arena_calloc(arena, count + 1, sizeof(char *)); */
/* 	dir = opendir("."); */
/* 	if (!dir) */
/* 		return (NULL); */
/* 	entry = readdir(dir); */
/* 	while (entry) */
/* 	{ */
/* 		*result++ = ft_arena_strndup( */
/* 				arena, entry->d_name, ft_strlen(entry->d_name)); */
/* 		entry = readdir(dir); */
/* 	} */
/* 	closedir(dir); */
/* 	return (result - count); */
/* } */
