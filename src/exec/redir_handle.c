/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 09:58:34 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/16 15:21:38 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* gere une seule redirection :
ouvre le fichier + affecte fd_in ou fd_out selon le type */
int	handle_single_redirection(t_cmd *cmd, t_redir *redir, t_shell *shell, t_exec *exec)
{
	int	fd;

	if (!redir)
		return (0);
	if (redir->type == TOKEN_REDIRECT_HEREDOC)
	{
		if (cmd->fd_in > 2)
			close(cmd->fd_in);
		fd = here_doc(exec, redir->file, cmd, shell);
		if (fd == -1)
			return (-1);
		cmd->fd_in = fd;
		return (0);
	}
	fd = open_file(redir->type, redir->file, shell);
	if (fd == -1)
		return (-1);
	if (redir->type == TOKEN_REDIRECT_IN)
	{
		if (cmd->fd_in > 2)
			close(cmd->fd_in);
		cmd->fd_in = fd;
	}
	else if (redir->type == TOKEN_REDIRECT_OUT || redir->type == TOKEN_REDIRECT_APPEND)
	{
		if (cmd->fd_out > 2)
			close(cmd->fd_out);
		cmd->fd_out = fd;
	}
	else
	{
		close(fd); // on ferme si type non prevu ??
		return (-1);
	}
	return (0);
}

/* applique toutes les redirections d'une commande. */
int	apply_redirections(t_cmd *cmd, t_shell *shell, t_exec *exec)
{
	t_redir	*redir;
	int		result;

	if (!cmd || !cmd->redir)
		return (0);
	redir = cmd->redir;
	while (redir)
	{
		result = handle_single_redirection(cmd, redir, shell, exec);
		if (result == -1)
			return (-1);
		redir = redir->next;
	}
	return (0);
}
