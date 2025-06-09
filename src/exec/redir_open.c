/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_open.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 08:22:07 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/09 16:00:11 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* ouvrir un fichier selon le type de redir demande. */
int	open_file(t_token type, char *file)
{
	int	fd;

	if (!file)
		return (-1);
	if (type == REDIR_IN)
		fd = open(file, O_RDONLY);
	else if (type == REDIR_OUT)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (type == REDIR_APPEND)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (-1); // pour HEREDOC
	if (fd == -1)
	{
		perror(file);
	}
	return (fd);
}
