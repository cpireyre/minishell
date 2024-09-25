#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

enum e_type
{
    WORD,
    META,
    SPACE,
    ERROR,
    END,
    NUM_TYPES,
};

typedef struct s_token
{
    const char	*data;
    size_t	size;
    enum e_type	type;
}	t_token;

enum e_type	token_get_type(char c)
{
    if (c == '<' || c == '>' || c == '&' || c == '|')
	return (META);
    else if (isspace(c))
	return (SPACE);
    else if (c == '\0')
	return (END);
    else
	return (WORD);
}

t_token	token_next(const char *str)
{
    t_token	result;
    size_t	i;

    result = (t_token){0};
    result.data = str;
    result.type = token_get_type(*str);
    if (result.type == END)
	return (result);
    while (token_get_type(*str) == result.type)
    {
	if (result.type == WORD && *str == '"')
	{
	    i = 1;
	    while (str[i] && str[i] != '"')
		i++;
	    if (str[i] != '"')
		return ((t_token){.data = result.data,
			.size = str - result.data + i, .type = ERROR});
	    str += i;
	}
	str++;
    }
    result.size = str - result.data;
    return (result);
}

t_token	*realloc_token_vector(t_token *xs, size_t capacity, size_t count)
{
    t_token		*tmp;

    tmp = malloc(sizeof(t_token) * capacity);
    printf("re-Allocated %zu bytes\n", sizeof(*xs) * capacity);
    if (!tmp)
    {
	free(xs);
	return (NULL);
    }
    if (xs)
	memcpy(tmp, xs, sizeof(*xs) * count);
    free(xs);
    return (tmp);
}

t_token	*tokenize(const char *str)
{
    t_token		*xs;
    const char		*ptr = str;
    const size_t	len = strlen(str);
    size_t		i;
    size_t		capacity;

    i = 0; 
    capacity = 1;
    xs = NULL;
    while (str - ptr <= len && i < INT_MAX)
    {
	if (i == capacity || !xs)
	{
	    capacity *= 2;
	    xs = realloc_token_vector(xs, capacity, i);
	    if (!xs)
		return (NULL);
	}
	while (isspace(*str))
	    str++;
	xs[i] = token_next(str);
	str += xs[i].size;
	if (xs[i++].type == END)
	    break ;
    }
    return (xs);
}

void	token_show(t_token token)
{
    static const char	*types[NUM_TYPES] = {
    "WORD",
    "META",
    "SPACE",
    "ERROR",
    "END",
    };

    for (size_t i = 0; i < token.size; i++)
	putchar(token.data[i]);
    printf(" (%s, %zu)\n", types[token.type], token.size);
}

int	main(int argc, char **argv)
{
    t_token	*xs;

    if (argc > 1)
    {
	xs = tokenize(argv[1]);
	if (xs)
	{
	    for (size_t i = 0; xs[i].type != END; i++)
		token_show(xs[i]);
	    puts("");
	    free(xs);
	}
	else
	    printf("ENOMEM??\n");
    }
}
