#include "libft.h"

/* exit [n] */
/*               Cause the shell to exit with a status of n.  If n is omitted, the  exit  status  is */
/*               that  of  the  last  command executed.  A trap on EXIT is executed before the shell */
/*               terminates. */
/* [...] */

/* bash-3.2$ exit 1 2 */
/* exit */
/* bash: exit: too many arguments */
/* echo $? */
/* 1 */

/* bash-3.2$ exit a */
/* exit */
/* bash: exit: a: numeric argument required */
/* colin与Granville向~/1/c/minishell說: echo $status                                                              一五:五二 */
/* 255 */

int	ft_parse_long(char *str, int64_t *n)
{
	int	i;
	int	sign;
	int	digits;

	i = 0;
	sign = -1;
	digits = 0;
	while (str[i])
	{
		if (str[i] == '+' || str[i] == '-')
			sign = i;
		digits += ft_isdigit(str[i]);
		i++;
	}
	if (!digits || digits != i - (sign == 0))
		return (0);
	*n = ft_atol(str);
	return (1);
}

int	builtin_exit(char **args)
{
	int		ok;
	int64_t	user_input;
	const char	*builtin_name = *args++;

	if (!args || !*args)
		return (0); // should return previous exit code
	ok = ft_parse_long(*args++, &user_input);
	if (*args)
	{
		ft_dprintf(2, "minishell: %s: too many arguments\n", builtin_name);
		return (1);
	}
	if (!ok)
	{
		ft_dprintf(2, "minishell: %s: numeric argument required\n", builtin_name);
		return (255);
	}
	if (DEBUG)
		ft_printf("Exit code: %d\n", (uint8_t)user_input);
	return ((uint8_t) user_input);
}
