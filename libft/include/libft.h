/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 12:48:12 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/07 18:52:55 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>
# include <stdint.h>
# include <sys/types.h>
# include "arena.h"
# include "ft_la.h"
# include "get_next_line.h"

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
	struct s_list	*prev;
}					t_list;

/* Mem */

void		ft_bzero(void *s, size_t n);
void		*ft_memset(void *b, int c, size_t len);
void		*ft_memchr(const void *s, int c, size_t n);
void		*ft_memcpy(void *dst, const void *src, size_t n);
void		*ft_memmove(void *dst, const void *src, size_t len);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
void		ft_memdel(void **ptr);
void		*ft_memset_32(void *b, uint32_t c, size_t len);
void		*ft_calloc(size_t elems, size_t elem_size);
void		ft_swap(int *a, int *b);

/* Str */

size_t		ft_strlen(const char *s);
char		*ft_strdup(const char *str);
char		*ft_strchr(const char *s, int c);
char		*ft_strrchr(const char *s, int c);
char		**ft_split(char const *s, char c);
char		*ft_strndup(const char *src, size_t n);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strtrim(char const *s1, char const *set);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t		ft_strlcat(char *dst, const char *src, size_t dstsize);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strnstr(const char *haystack, const char *needle, size_t len);
char		*ft_strtok(char *str, const char *sep);
int			ft_streq(const char *s1, const char *s2);
const char	*ft_strchrnul(const char *str, int c);
char		*ft_arena_strndup(t_arena arena, const char *src, size_t n);

/* Ctype */

int			ft_isdigit(int c);
int			ft_isspace(int c);
int			ft_atoi(const char *str);
long		ft_atol(const char *str);
int			ft_parse_int(char *str, long *n);
int			ft_isalnum(char c);

/* IO */

ssize_t		ft_gnl(int fd, char **line);
int			ft_error(void *data, const char *msg);
int			ft_printf(const char *format, ...);
int			ft_dprintf(int fd, const char *format, ...);
int			ft_snprintf(char *str, size_t size, const char *format, ...);
char		*ft_allocaprintf(const char *format, ...);

/* Math */

int			ft_min4(int a, int b, int c, int d);
size_t		ft_size_t_min(size_t a, size_t b);

/* Lst */

t_list		*ft_lstnew(void *content);
void		ft_lstadd_front(t_list **lst, t_list *new);
int			ft_lstsize(t_list *lst);
t_list		*ft_lstlast(t_list *lst);
void		ft_lstadd_back(t_list **lst, t_list *new);
void		ft_lstdelone(t_list *lst, void (*del)(void *));
void		ft_lstclear(t_list **lst, void (*del)(void *));
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

#endif
