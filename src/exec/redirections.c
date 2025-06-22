/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:20:15 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/22 09:04:27 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* applique les dup2 pour rediriger in et out
changement condi car heredoc en pipe, pipe_fd[0] == 0
donc je dois faire dup2() si fd_in != STDIN_FILENO
*/
void	apply_dup_redirections(t_cmd *cmd)
{
	if (cmd->fd_in != STDIN_FILENO)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
		{
			perror("dup2 (input)");
			exit(1);
		}
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
		{
			perror("dup2 (output)");
			exit(1);
		}
	}
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

int	apply_redirections(t_cmd *cmd, t_shell *shell)
{
	if (!cmd || !cmd->redir)
		return (0);
	if (check_redirections_consistency(cmd, shell) == -1)
		return (-1);
	apply_dup_redirections(cmd);
	close_redirections(cmd);
	return (0);
}

