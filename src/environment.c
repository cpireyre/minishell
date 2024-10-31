/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 13:10:23 by pleander          #+#    #+#             */
/*   Updated: 2024/10/31 13:19:51 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arena.h"
#include "libft.h"
#include "minishell.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Gets the element in the list where the variable is stored
 *
 * @param var environmental variable to find
 * @param env list of environmental variables
 * @return pointer to list element or NULL if variable does not exist
 */
t_list	*get_env_list_location(char *var, t_list **env)
{
	int		keylen;
	t_list	*tmp;

	tmp = *env;
	keylen = ft_strlen(var);
	while (tmp)
	{
		if (!ft_strncmp(var, tmp->content, keylen)
			&& ((char *)tmp->content)[keylen] == '=')
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

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
	t_list	*loc;
	char	*value;

	loc = get_env_list_location(var, env);
	if (!loc)
		return (NULL);
	value = loc->content + ft_strlen(var) + 1;
	return (value);
}

static char	*make_env_str(char *var, char *val)
{
	size_t	str_len;
	char	*env_str;

	str_len = (ft_strlen(var) + ft_strlen(val) + 2);
	env_str = ft_calloc(str_len, sizeof(char));
	if (!env_str)
		return (NULL);
	ft_strlcat(env_str, var, str_len);
	ft_strlcat(env_str, "=", str_len);
	ft_strlcat(env_str, val, str_len);
	return (env_str);
}

int	set_env(char *var, char *val, t_list **env)
{
	t_list	*loc;
	t_list	*new;
	char	*env_str;

	env_str = make_env_str(var, val);
	if (!env_str)
		return (1);
	loc = get_env_list_location(var, env);
	if (!loc)
	{
		new = ft_lstnew(env_str);
		if (!new)
		{
			free(env_str);
			return (1);
		}
		ft_lstadd_back(env, new);
	}
	else
	{
		free(loc->content);
		loc->content = env_str;
	}
	return (0);
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
	if (!env)
		return (NULL);
	ft_bzero(env, sizeof(env));
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

char **make_raw_env_array(t_list *env, t_arena arena)
{
	char	**renv;
	size_t	i;
	const size_t	env_size = ft_lstsize(env);

	renv = arena_calloc(arena, env_size + 1, sizeof(char *));
	if (!renv)
		return (NULL);
	i = 0;
	while (i < env_size)
	{
		renv[i] = env->content;
		env = env->next;
		i++;
	}
	return (renv);
}
