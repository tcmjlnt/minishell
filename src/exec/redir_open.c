/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_open.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 08:22:07 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/09 18:40:01 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* ouvrir un fichier selon le type de redir demande. */
int	open_file(t_token_type type, char *file)
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
	}
	return (fd);
}
