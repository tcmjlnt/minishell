/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_open.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 08:22:07 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/13 13:52:43 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* ouvrir un fichier selon le type de redir demande. */
int	open_file(t_token_type type, char *file, t_shell *shell)
{
	int	fd;

	if (!file)
		return (-1);
	if (type == TOKEN_REDIRECT_IN)
	{
		if (is_directory(file))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(file, STDERR_FILENO);
			ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
			shell->exit_status = 126;
			return (-1);
		}
		if (check_file_access(file, R_OK, 126, shell, ": Permission denied\n") == -1)
			return (-1);
		fd = open(file, O_RDONLY);
	}
	else if (type == TOKEN_REDIRECT_OUT || type == TOKEN_REDIRECT_APPEND)
	{
		if (access(file, F_OK) == 0)
		{
			if (is_directory(file))
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				ft_putstr_fd(file, STDERR_FILENO);
				ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
				shell->exit_status = 126;
				return (-1);
			}
			if (check_file_access(file, W_OK, 126, shell, ": Permission denied\n") == -1)
				return (-1);
		}
		if (type == TOKEN_REDIRECT_OUT)
			fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else
		return (-1); // pour HEREDOC
	if (fd == -1)
	{
		perror(file);
		shell->exit_status = 1;
	}
	return (fd);
}

/* verifier si le fichier est un repertoire */
int	is_directory(char *file)
{
	int	fd;

	fd = open(file, O_DIRECTORY);
	if (fd < 0)
		return (0);
	close(fd);
	return (1);
}

int	check_file_access(char *file, int mode, int exit_status, t_shell *shell, const char *msg)
{
	if (access(file, mode) == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(file, STDERR_FILENO);
		ft_putstr_fd(msg, STDERR_FILENO);
		shell->exit_status = exit_status;
		return (-1);
	}
	return (0);
}
