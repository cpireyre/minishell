/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: copireyr <copireyr@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 12:46:57 by copireyr          #+#    #+#             */
/*   Updated: 2024/11/18 11:45:45 by copireyr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

int		*exit_code(void);
int		set_signal_handler(void);
int		set_nop_handler(void);

#endif /* SIGNALS_H */
