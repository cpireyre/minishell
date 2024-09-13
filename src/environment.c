/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 13:10:23 by pleander          #+#    #+#             */
/*   Updated: 2024/09/13 13:04:20 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Gets the value of an environmental variable
 *
 * @param var Environmental variable to get
 * @param env List of environment variables
 * @return Pointer to the location of the value in the list of NULL if
 * if does not exist
 */
char	*get_env(char *var, t_list **env)
{
	char	*key;
	int		keylen;

	key = ft_strjoin(var, "=");
	keylen = ft_strlen(key);
	if (!env)
		return (NULL);
	while (*env)
	{
		if (ft_strncmp(key, (*env)->content, keylen) == 0)
			return ((*env)->content + keylen);
		*env = (*env)->next;
	}
	return (NULL);
}

/**
 * @brief Initializes a new list of environmental variables
 *
 * @param envp Array of environmental variables, passed to main as third argument
 * @return pointer to list of environmental variables, otherwise NULL
 */
t_list	**init_env(char **envp)
{
	t_list	**env;
	t_list	*new;
	char	*envstr;

	if (!envp || !*envp)
		return (NULL);
	env = malloc(sizeof(t_list *));
	ft_bzero(env, sizeof(*env));
	if (!env)
		return (NULL);
	while (*envp)
	{
		envstr = ft_strdup(*envp);
		if (!envstr)
			return (NULL); //Free allocated stuff first
		new = ft_lstnew(envstr);
		if (!new)
			return (NULL); //Free allocated stuff first
		ft_lstadd_back(env, new);
		envp++;
	}
	return (env);
}
