/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pleander <pleander@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 18:31:06 by pleander          #+#    #+#             */
/*   Updated: 2024/11/13 18:32:17 by pleander         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "execute.h"

void	print_command(t_command *cmd)
{
	int	i;

	ft_dprintf(2, "[%s]:", cmd->path);
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		ft_dprintf(2, " (%d: %s)", i, cmd->args[i]);
		i++;
	}
	if (cmd->infile)
		ft_dprintf(2, " (< %s)", cmd->infile);
	if (cmd->outfile)
		ft_dprintf(2, " (> %s)", cmd->outfile);
	ft_dprintf(2, "\n");
}

int	is_path(char	*command)
{
	if (command && ft_strchr("./", command[0]))
		return (1);
	return (0);
}
