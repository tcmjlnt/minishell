/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks_full_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 10:07:36 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/25 22:09:13 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	open_file(t_redir *redir, t_shell *shell)
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
		error_exit("open_file : redir file error : fd = -1");
	return (fd);
}

int	check_redirections_consistency(t_cmd *cmd, t_shell *shell)
{
	t_redir	*redir_list;

	redir_list = cmd->redir;
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	while (redir_list)
	{
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

t_bool	is_directory(char *file)
{
	int	fd;

	fd = open(file, O_DIRECTORY);
	if (fd < 0)
		return (false);
	close(fd);
	return (true);
}

t_bool	is_valid_command(t_cmd *cmd, t_shell *shell, int *status, char **path)
{
	if (!cmd->cmd)
		return (false);
	else if (cmd->cmd[0] == '\0')
	{
		ft_putstr_fd(cmd->cmd, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		*status = 127;
		return (false);
	}
	else if (is_directory(cmd->cmd))
	{
		ft_putstr_fd(cmd->cmd, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		*status = 126;
		return (false);
	}
	*path = find_command_path(cmd->cmd, shell->env);
	if (!cmd->is_builtin && *path == NULL)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->cmd, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		*status = 127;
		return (false);
	}
	return (true);
}
