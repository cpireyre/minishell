#include "minishell.h"

int	main(void)
{
	t_pipeline p;
	t_command c[2];
	char *no_args = "";

	c[0].i = 0;
	c[0].path = "/usr/bin/cat";
	c[0].args = &no_args;
	c[0].infile = "/home/peter/code/hive/minishell/repo/src/infile";
	c[0].outfile = NULL;
	c[0].hdoc_delim = NULL;
	c[0].of_append = 0;
	c[0].exit_code = 0;
	
	c[0].i = 1;
	c[0].path = "/usr/bin/cat";
	c[0].args = &no_args;
	c[0].infile = NULL;
	c[0].outfile = "/home/peter/code/hive/minishell/repo/src/outfile";
	c[0].hdoc_delim = NULL;
	c[0].of_append = 0;
	c[0].exit_code = 0;

	p.cmds = c;
	p.n_cmds = 2;

	printf("Return code: %d\n", pipeline(&p));
}
