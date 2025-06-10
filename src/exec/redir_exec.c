/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 09:58:47 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/10 10:34:10 by aumartin         ###   ########.fr       */
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
/*	prepare_child(cmd, shell)
	↓
	execute builtin ou exec_external_cmd(cmd, shell)
	↓
	fin du process enfant */

void	prepare_child(t_cmd *cmd, t_shell *shell)
{
	if (apply_redirections(cmd) == -1)
	{
		shell->exit_status = 1;
		exit(shell->exit_status);
	}
	apply_dup_redirections(cmd);
}

/* ferme les fd ouverts pour les redir
fd_in et fd_out mis -1 => permet de savoir etat aps qu'ils sont fermes
et donc eviter de les refermer par erreur.
*/
void	close_redirections(t_cmd *cmd)
{
	if (cmd->fd_in > 2)
	{
		close(cmd->fd_in);
		cmd->fd_in = -1;
	}
	if (cmd->fd_out > 2)
	{
		close(cmd->fd_out);
		cmd->fd_out = -1;
	}
}
