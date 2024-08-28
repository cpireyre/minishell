/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 12:54:00 by copireyr          #+#    #+#             */
/*   Updated: 2024/07/18 13:12:16 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	consume_input(int fd, t_gnl_list *node, t_gnl_list *leftovers);
static char	*list_to_string(t_gnl_list *head, size_t total_size);
static int	find_newline(char *str, int size);
static void	*list_free(t_gnl_list *head);

char	*get_next_line(int fd)
{
	t_gnl_list			*head;
	t_gnl_list			*tail;
	size_t				total_size;
	static t_gnl_list	leftovers;
	t_gnl_list			*node;

	head = NULL;
	node = NULL;
	tail = NULL;
	total_size = 0;
	while (!node || (node->newline_index == -1 && node->size > 0))
	{
		node = ft_calloc(1, sizeof(*node));
		if (!node)
			return (list_free(head));
		if (!head)
			head = node;
		if (tail)
			tail->next = node;
		tail = node;
		if (consume_input(fd, node, &leftovers) < 0)
			return (list_free(head));
		total_size += (size_t)node->size;
	}
	return (list_to_string(head, total_size));
}

static int	consume_input(int fd, t_gnl_list *node, t_gnl_list *leftovers)
{
	if (leftovers->size > 0)
	{
		ft_memcpy(node->chunk, leftovers->chunk, (size_t)leftovers->size);
		node->size = leftovers->size;
		leftovers->size = 0;
	}
	else
		node->size = (int)read(fd, node->chunk, BUFFER_SIZE);
	if (node->size > 0)
	{
		node->newline_index = find_newline(node->chunk, node->size);
		if (node->newline_index != -1)
		{
			leftovers->size = node->size - node->newline_index - 1;
			ft_memcpy(leftovers, node->chunk + node->newline_index + 1,
				(size_t)leftovers->size);
			node->size -= leftovers->size;
		}
	}
	return (node->size);
}

static char	*list_to_string(t_gnl_list *head, size_t total_size)
{
	int			current_size;
	char		*line;
	t_gnl_list	*node;

	if (total_size <= 0)
		return (list_free(head));
	line = malloc(total_size + 1);
	if (!line)
		return (list_free(head));
	current_size = 0;
	node = head;
	while (node)
	{
		if (node->size > 0)
		{
			ft_memcpy(line + current_size, node->chunk, (size_t)node->size);
			current_size += node->size;
		}
		node = node->next;
	}
	line[total_size] = '\0';
	list_free(head);
	return (line);
}

static int	find_newline(char *str, int size)
{
	int	newline_index;

	newline_index = 0;
	while (newline_index < size && str[newline_index] != '\n')
		newline_index++;
	if (newline_index < size && str[newline_index] == '\n')
		return (newline_index);
	return (-1);
}

static void	*list_free(t_gnl_list *head)
{
	t_gnl_list	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
	return (NULL);
}
