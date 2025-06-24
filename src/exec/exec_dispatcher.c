/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_dispatcher.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 13:45:46 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/24 19:11:55 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exec_dispatcher(t_cmd *cmd, t_shell *shell)
{
	if (!cmd)
		return ;
	if (handle_all_heredocs(cmd) == -1)
		return ;
	signal(SIGINT, SIG_IGN);
	if (!cmd->next)
		exec_single_cmd(cmd, shell);
	else
		exec_pipeline(cmd, shell);
}
