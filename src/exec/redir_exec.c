/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 09:58:47 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/09 15:58:03 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* applique les dup2 pour rediriger in et out
if ( > 2) => ce n’est pas les STDIN_FILENO (0), STDOUT_FILENO (1), STDERR_FILENO (2).
*/

void	apply_dup_redirections(t_cmd *cmd)
{
	if (cmd->fd_in > 2)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
		{
			perror("dup2 (input)");
			exit(1);
		}
	}
	if (cmd->fd_out > 2)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
		{
			perror("dup2 (output)");
			exit(1);
		}
	}
}
