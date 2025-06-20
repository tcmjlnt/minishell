/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 09:58:34 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/20 19:13:34 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_heredoc(t_cmd *cmd, t_redir *redir, t_shell *shell)
{
	int	fd;

	if (cmd->fd_in > 2)
		close(cmd->fd_in);
	fd = here_doc(redir->file, cmd, shell);
	if (fd == -1)
	{
		shell->exit_status = 1;
		return (-1);
	}
	cmd->fd_in = fd;
	return (0);
}

static int	handle_infile(t_cmd *cmd, char *file, t_shell *shell)
{
	int	fd;

	if (cmd->fd_in > 2)
		close(cmd->fd_in);
	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		perror(file);
		shell->exit_status = 1;
		return (-1);
	}
	cmd->fd_in = fd;
	return (0);
}

static int	handle_outfile(t_cmd *cmd, char *file, int append, t_shell *shell)
{
	int	fd;
	int	flags;

	if (append == 1)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	if (cmd->fd_out > 2)
		close(cmd->fd_out);
	fd = open(file, flags, 0644);
	if (fd == -1)
	{
		perror(file);
		shell->exit_status = 1;
		return (-1);
	}
	cmd->fd_out = fd;
	return (0);
}

/* gere une seule redirection :
ouvre le fichier + affecte fd_in ou fd_out selon le type */
int	handle_single_redirection(t_cmd *cmd, t_redir *redir, t_shell *shell)
{
	if (!redir)
		return (0);
	if (redir->type == TOKEN_REDIRECT_HEREDOC)
		return (handle_heredoc(cmd, redir, shell));
	else if (redir->type == TOKEN_REDIRECT_IN)
		return (handle_infile(cmd, redir->file, shell));
	else if (redir->type == TOKEN_REDIRECT_OUT)
		return (handle_outfile(cmd, redir->file, 0, shell));
	else if (redir->type == TOKEN_REDIRECT_APPEND)
		return (handle_outfile(cmd, redir->file, 1, shell));
	ft_putstr_fd("minishell: unsupported redirection type\n", STDERR_FILENO);
	shell->exit_status = 2;
	return (-1);
}

/* applique toutes les redirections d'une commande. */
int	apply_redirections(t_cmd *cmd, t_shell *shell)
{
	t_redir	*redir;

	if (!cmd || !cmd->redir)
		return (0);
	redir = cmd->redir;
	while (redir)
	{
		if (handle_single_redirection(cmd, redir, shell) == -1)
			return (-1);
		redir = redir->next;
	}
	return (0);
}
