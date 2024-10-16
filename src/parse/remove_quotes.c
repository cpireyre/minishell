#include "ast.h"

char	*remove_quotes_from_str(t_arena arena, const char *str)
{
	size_t	i;
	size_t	size;
	char	*result;

	i = 0;
	size = 0;
	while (str[i])
	{
		if (str[i] == '"')
		{
			i++;
			while (str[i] && str[i] != '"')
			{
				size++;
				i++;
			}
		}
		if (str[i] == '\'')
		{
			i++;
			while (str[i] && str[i] != '\'')
			{
				size++;
				i++;
			}
		}
		size += str[i] != '"' && str[i] != '\'';
		i++;
	}
	result = arena_alloc(arena, size + 1);
	if (!result)
		return (NULL);
	while (*str)
	{
		if (*str != '"' && *str != '\'')
			*result++ = *str;
		else if (*str == '"')
		{
			str++;
			while (*str && *str != '"')
			*result++ = *str++;
		}
		else if (*str == '\'')
		{
			str++;
			while (*str && *str != '\'')
			*result++ = *str++;
		}
		str++;
	}
	*result = '\0';
	return (result - size);
}

/* TODO: Check return value for ENOMEM */
void	remove_quotes(t_arena arena, t_ast_node *ast)
{
	size_t		i;
	const char	*str;

	if (!ast)
		return ;
	i = 0;
	while (i < ast->n_children)
	{
		if (ast->children[i]->type == AST_WORD)
		{
			str = remove_quotes_from_str(arena, ast->children[i]->token.value);
			ast->children[i]->token.value = str;
			ast->children[i]->token.size = ft_strlen(str);
		}
		remove_quotes(arena, ast->children[i]);
		i++;
	}
}
