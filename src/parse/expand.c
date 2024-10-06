/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 10:00:45 by copireyr          #+#    #+#             */
/*   Updated: 2024/10/06 11:48:17 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "ast.h"

char	*val(t_list *env, const char *key, size_t length_key)
{
    while (env)
    {
	size_t i = 0;
	char *curr = env->content;
	while (curr[i] != '=')
	    i++;
	if (i == length_key && !ft_memcmp(key, curr, length_key))
	    return (curr + i + 1);
	env = env->next;
    }
    return (NULL);
}

/* The interface probably ought to be:
 * char * -> explode into char ** -> join back into char *
 */
void	expand(t_ast_node *ast, t_arena arena, t_list *env)
{
    if (!ast)
	return ;
    if (ast->type == AST_COMMAND)
    {
	const char *str = ast->token.value;
	while (*str)
	{
	    while (*str && *str != '$')
	    {
		if (*str == '\'')
		    str = ft_strchrnul(str + 1, '\'');
		str++;
	    }
	    if (*str++ != '$')
		break ;
	    size_t i = 0;
	    while (ft_isalnum(str[i]) || str[i] == '_')
		i++;
	    char *expandable = arena_calloc(arena, 1, i + 1);
	    if (expandable)
		ft_memcpy(expandable, str, i);
	    str += i;
	    const size_t    length_expandable = ft_strlen(expandable);
	    ft_printf("Found expandable: [%s]\n", expandable);
	    const char *match = val(env, expandable, length_expandable);
	    if (match)
		ft_printf("%s -> %s\n", expandable, match);
	}
    }
    for (size_t i = 0; i < ast->n_children; i++)
	expand(ast->children[i], arena, env);
}
