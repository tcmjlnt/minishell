/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_open.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 08:22:07 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/13 14:08:32 by aumartin         ###   ########.fr       */
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
		fd = open(file, O_RDONLY);
	else if (type == TOKEN_REDIRECT_OUT)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == TOKEN_REDIRECT_APPEND)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
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

