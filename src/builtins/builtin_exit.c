/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 22:04:13 by copireyr          #+#    #+#             */
/*   Updated: 2024/11/15 10:13:25 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_shell_status	builtin_exit(char **args, int prev_exit)
{
	int				ok;
	int64_t			user_input;
	const char		*builtin_name = *args++;
	char			*argument;

	if (!args || !*args)
		return ((t_shell_status){.exit_code = prev_exit, .should_exit = true});
	argument = *args++;
	ok = ft_parse_long(argument, &user_input);
	if (*args)
	{
		ft_dprintf(2, NAME ": %s: too many arguments\n", builtin_name);
		return ((t_shell_status){.exit_code = 1, .should_exit = false});
	}
	if (!ok)
	{
		ft_dprintf(2, NAME ": %s: %s: numeric argument required\n", builtin_name, argument);
		return ((t_shell_status){.exit_code = 2, .should_exit = true});
	}
	if (DEBUG)
		ft_printf("Exit code: %d\n", (uint8_t)user_input);
	return ((t_shell_status){
		.exit_code = (uint8_t) user_input, .should_exit = true});
}
