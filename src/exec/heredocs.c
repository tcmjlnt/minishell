/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 12:53:38 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/24 21:32:49 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*gen_tmp_filename(void)
{
	static int	id = 0;
	char		*id_str;
	char		*tmp_path;
	char		*filename;

	tmp_path = NULL;
	id_str = gc_itoa(id++, GC_TMP);
	if (!id_str)
		return (NULL);
	tmp_path = gc_strjoin("/tmp/minishell_heredoc_", id_str, GC_TMP);
	if (!tmp_path)
		return (NULL);
	filename = gc_strjoin(tmp_path, ".tmp", GC_TMP);
	if (!filename)
		return (NULL);
	return (filename);
}

int	handle_heredoc(t_redir *redir)
{
	char	*line;
	//char	*filename;
	int		fd;
	size_t	len;

	len = ft_strlen(redir->delim);
	redir->file = gen_tmp_filename();
	if (!redir->file)
		return (perror("malloc name"), -1);
	fd = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("open heredoc temp");
		// gc_mem(GC_FREE_ALL, 0, NULL, GC_TMP);
		return (-1);
	}
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
			ft_putstr_fd("", STDERR_FILENO);
		if (ft_strncmp(line, redir->delim, len) == 0 && line[len] == '\0')
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
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
