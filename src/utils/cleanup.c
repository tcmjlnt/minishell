/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 07:54:42 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/26 07:55:54 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_and_cleanup_heredocs(t_cmd *cmd_list)
{
	cleanup_heredocs(cmd_list);
	gc_mem(GC_FREE_ALL, 0, NULL, GC_TMP);
}

