/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 12:53:38 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/21 18:13:07 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*gen_tmp_filename(void)
{
	static int	id = 0;
	char		*name;
	char		*id_str;

	id_str = ft_itoa(id++);
	if (!id_str)
		return (NULL);
	name = malloc(40);
	if (!name)
		return (free(id_str), NULL);
	ft_strlcpy(name, "/tmp/minishell_heredoc_", (ft_strlen(name) + 24));
	ft_strlcat(name, id_str, (ft_strlen(name) + ft_strlen (id_str) + 1));
	ft_strlcat(name, ".tmp", (ft_strlen(name) + 5));
	free(id_str);
	return (name);
}

int	handle_heredoc(t_redir *redir)
{
	char	*line;
	//char	*filename;
	int		fd;
	size_t	len;

	redir->limiter = ft_strdup("DEL"); // A ENLEVER
	len = ft_strlen(redir->limiter);
	redir->file = gen_tmp_filename();
	if (!redir->file)
		return (perror("malloc name"), -1);
	fd = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
		return (perror("open heredoc temp"), free(redir->file), -1); //GC
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
			break ;
		if (ft_strncmp(line, redir->limiter, len) == 0 && line[len] == '\0')
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	//free(line);
	close(fd);
	return (0); // verif le exit status
}

int	handle_all_heredocs(t_cmd *cmd_list)
{
	t_cmd	*cmd;
	t_redir	*redir;

	cmd = cmd_list;
	while (cmd)
	{
		redir = cmd->redir;
		while (redir)
		{
			if (redir->type == TOKEN_REDIRECT_HEREDOC)
			{
				if (handle_heredoc(redir) == -1)
					return (1);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
