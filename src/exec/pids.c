/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pids.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 19:25:09 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/04 19:25:26 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static pid_t	*alloc_pids(int cmd_count)
{
	pid_t *pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		error_exit("malloc pids");
	return (pids);
}
