#include "tokenize.h"

void	tokenize_show_tokens(t_token *xs)
{
	static const char	*types[NUM_TYPES] = {
		"WORD", "APPEND", "REDIRECT_IN", "REDIRECT_OUT",
		"HEREDOC", "LOGICAL_AND", "LOGICAL_OR", "PIPE",
		"META", "TOKENIZE_SPACE", "ERROR", "END",
	};
	size_t				i;
	size_t				j;

	j = 0;
	while (xs[j].type != END)
	{
		ft_printf("[");
		i = 0;
		while (i < xs[j].size)
			ft_printf("%c", xs[j].data[i++]);
		ft_printf(" (%s)] ", types[xs[j].type]);
		j++;
	}
	ft_printf("\n");
}

void	tokenize_test(void)
{
	const char	*tests[] = {"ls", "echo a", "date > outfile",
		"| ah &&", "echo\"a   b   c\"", "\"a\"", "\"a",
		"echo\"a b c \"'this should be one though   'still   tok",
		"cat << EOF hello heredoc >>",
		"cat < infile > outfile",
		NULL};
	t_token		*xs;
	size_t		j;

	ft_dprintf(2, "Running tokenizer tests...\n");
	j = 0;
	while (tests[j])
	{
		ft_printf("Tokenizing: %s\n", tests[j]);
		xs = tokenize(tests[j]);
		if (xs)
		{
			tokenize_show_tokens(xs);
			free(xs);
		}
		else
			ft_dprintf(2, "ENOMEM??\n");
		j++;
	}
}

