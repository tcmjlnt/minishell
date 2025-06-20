/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 12:53:38 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/20 09:10:47 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// utiliser readline au lieu de gnl
// tout foutre dans un tmp ??
// a implenter apres signaux, voir Eric ??

/* lit une ligne avec readline */
static char	*read_heredoc_line(void)
{
	char	*line;

	line = readline("heredoc> ");
	if (!line)
		return (NULL);
	return (line);
}

/* join toutes les lignes jusqu'au limiter dans GC_TMP => content */
static char	*read_heredoc_content(char *limiter)
{
	char	*line;
	char	*content;
	size_t	len;

	content = NULL;
	len = ft_strlen(limiter);
	while (1)
	{
		line = read_heredoc_line();
		if (!line)
			break ;
		if (ft_strncmp(line, limiter, len) == 0 && line[len] == '\0')
			break ;
		content = gc_strjoin(content, line, GC_TMP);
		content = gc_strjoin(content, "\n", GC_TMP);
	}
	return (content);
}

/* Base de Pipex - cree un pipe, remplit content et return fd de lecture */
int	here_doc(char *limiter, t_cmd *cmd, t_shell *shell)
{
	int		pipe_fd[2];
	char	*content;

	if (!limiter || !cmd || !shell)
		return (-1);
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		shell->exit_status = 1;
		return (-1);
	}
	content = read_heredoc_content(limiter);
	if (content)
		write(pipe_fd[1], content, ft_strlen(content));
	close(pipe_fd[1]);
	cmd->fd_in = pipe_fd[0];
	return (0);
}

/* Vérifie les erreurs potentielles liées aux heredocs dans une commande :
   - limiter manquant
   - limiter vide (<< "")
   - nombre excessif de heredocs (limite à 10 ? message explicatif) */
t_bool	check_heredoc_errors(t_cmd *cmd)
{
	t_redir	*redir;
	int		heredoc_count;

	redir = cmd->redir;
	heredoc_count = 0;
	while (redir)
	{
		if (redir->type == TOKEN_REDIRECT_HEREDOC)
		{
			heredoc_count++;
			if (!redir->file)
			{
				ft_putstr_fd("minishell: heredoc: missing limiter\n", STDERR_FILENO);
				return (false);
			}
			if (redir->file[0] == '\0')
			{
				ft_putstr_fd("minishell: heredoc: empty limiter not allowed\n", STDERR_FILENO);
				return (false);
			}
			if (heredoc_count > 10) // limite à 10 ? je voudrais meme mettre a 3 car c'est pas interessant de faire ca a tester
			{
				ft_putstr_fd("minishell: too many heredocs (limit: 10)\n", STDERR_FILENO);
				return (false);
			}
		}
		redir = redir->next;
	}
	return (true);
}

/* Vérifie tous les heredocs de toutes les commandes avant exécution.
   Retourne false s’il y a une erreur, et fixe shell->exit_status */
t_bool	check_all_heredocs(t_cmd *cmds, t_shell *shell)
{
	t_cmd *current;

	current = cmds;
	while (current)
	{
		if (!check_heredoc_errors(current))
		{
			shell->exit_status = 2;
			return (false);
		}
		current = current->next;
	}
	return (true);
}
