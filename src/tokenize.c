/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 12:46:17 by copireyr          #+#    #+#             */
/*   Updated: 2024/09/13 13:00:40 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*next_token(char *str);

void	tokenize(char *str)
{
	char	*current_token;

	ft_printf("Hello from tokenizer\n");
	ft_printf("Input string: %s\n", str);
	while (1)
	{
		current_token = next_token(str);
		if (!current_token)
			break ;
		ft_printf("Current token: %s\n", current_token);
	}
}

char	*next_token(char *str)
{
	if (!str || !*str)
		return (NULL);
	if (ft_isspace(*str))
		while (*str && ft_isspace(*str++))
			;
	else
		while (*str && !ft_isspace(*str++))
			;
	return (str);
}
