/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pids.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 19:25:09 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/05 07:40:34 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* stocker les pid dans t_cmd ??

manip les processus après leur fork  ?? faire des kill(pid)
recup les exit_status ??

*/

pid_t	*alloc_pids(int cmd_count)
{
	pid_t *pids;

	pids = gc_malloc(sizeof(pid_t) * cmd_count, GC_CMD);
	if (!pids)
		error_exit("malloc pids");
	return (pids);
}
