/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks_full_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 10:07:36 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/22 15:54:11 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_bool	is_valid_command(t_cmd *cmd, t_shell *shell)
{
	if (!cmd || !cmd->cmd || cmd->cmd[0] == '\0')
		return (false);
	if (is_directory(cmd->cmd))
		return (false);
	if (!cmd->is_builtin && !find_command_path(cmd->cmd, shell->env))
		return (false);
	return (true);
}

t_bool	check_invalid_cmds(t_cmd *cmd, t_shell *shell)
{
	t_cmd	*current;

	current = cmd;
	while (current)
	{
		if (!current->is_builtin && !is_valid_command(current, shell))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			if (current->cmd)
				ft_putstr_fd(current->cmd, STDERR_FILENO);
			else
				ft_putstr_fd("(null)", STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			shell->exit_status = 127;
			return (false);
		}
		current = current->next;
	}
	return (true);
}

int	open_file(t_redir *redir, t_shell *shell)
{
	int	fd;

	fd = -1;
	if (!redir->file)
		return (-1);
	if (redir->type == TOKEN_REDIRECT_IN || redir->type == TOKEN_REDIRECT_HEREDOC)
		fd = open(redir->file, O_RDONLY);
	else if (redir->type == TOKEN_REDIRECT_OUT)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == TOKEN_REDIRECT_APPEND)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(redir->file);
		shell->exit_status = 1;
	}
	return (fd);
}

int	check_redirections_consistency(t_cmd *cmd, t_shell *shell)
{
	t_redir	*redir_list;

	redir_list = cmd->redir;
	// c'est deja leur init de parsing mais a chacker si c'est bon
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	while (redir_list)
	{
		/* int fd = handle_redir(redir_list);
		if (fd == -1)
			return (perror("redirection"), 1); */
		if (redir_list->type == TOKEN_REDIRECT_IN || redir_list->type == TOKEN_REDIRECT_HEREDOC)
		{
			if (cmd->fd_in != STDIN_FILENO)
				close(cmd->fd_in);
			cmd->fd_in = open_file(redir_list, shell);
			if (cmd->fd_in == -1)
				return (-1);
		}
		else if (redir_list->type == TOKEN_REDIRECT_OUT || redir_list->type == TOKEN_REDIRECT_APPEND)
		{
			if (cmd->fd_out != STDOUT_FILENO)
				close(cmd->fd_out);
			cmd->fd_out = open_file(redir_list, shell);
			if (cmd->fd_out == -1)
				return (-1);
		}
		redir_list = redir_list->next;
	}
	return (0);
}
