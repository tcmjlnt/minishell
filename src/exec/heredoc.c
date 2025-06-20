/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 12:53:38 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/20 20:03:51 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// utiliser readline au lieu de gnl
// tout foutre dans un tmp ??
// a implenter apres signaux, voir Eric ??

/* lit une ligne avec readline  */
int	*read_heredoc_content(char *limiter, int fd)
{
	char	*line;
	size_t	len;

	len = ft_strlen(limiter);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
			break ;
		if (ft_strncmp(line, limiter, len) == 0 && line[len] == '\n')
			break ;
		ft_putendl_fd(line, fd);
		free(line);
	}
	return (0);
}

/* cree le heredoc le stock dans un tmp */
int	here_doc(char *limiter, t_cmd *cmd, t_shell *shell)
{
	int		heredoc_fd_in;
	int		heredoc_fd_out;
	char	*content;
	pid_t	pid;

	if (!limiter || !cmd || !shell)
		return (-1);
	pid = fork();
	if (pid == -1)
		error_exit("fork");
	content = read_heredoc_content(limiter, heredoc_fd_in);
	if (content)
	{
		ft_putstr_fd(content, heredoc_fd_out);
	}
	heredoc_fd_in = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	close(heredoc_fd_out);
	cmd->fd_in = heredoc_fd_in;
	return (0);
}

t_redir	*get_good_redir_out(t_cmd *cmd, t_shell *shell)
{
	t_redir	*redir_cur;
	int		fd_in;
	int		fd_out;

	redir_cur = cmd->redir;
	while (redir_cur)
	{
		if (redir_cur->next)
		{
			redir_cur = redir_cur->next;
			printf("j'ai un suivant, je passe au suivant");
		}
		printf("je n'ai plus de suivant");
	}
	while (redir_cur)
	{
		if (redir_cur->type == TOKEN_REDIRECT_OUT || redir_cur->type == TOKEN_REDIRECT_APPEND)
		{
			printf("je suis un redir_out");
			break ;
		}
		redir_cur = redir_cur->previous;
	}
	return (redir_cur);
}

t_redir	*get_good_redir_in(t_cmd *cmd, t_shell *shell)
{
	t_redir	*redir_cur;
	int		fd_in;
	int		fd_out;

	redir_cur = cmd->redir;
	while (redir_cur)
	{
		if (redir_cur->next)
		{
			redir_cur = redir_cur->next;
			printf("j'ai un suivant, je passe au suivant");
		}
		printf("je n'ai plus de suivant");
	}
	while (redir_cur)
	{
		if (redir_cur->type == TOKEN_REDIRECT_IN || redir_cur->type == TOKEN_REDIRECT_HEREDOC)
		{
			printf("je suis un redir_in");
			break ;
		}
		redir_cur = redir_cur->previous;
	}
	return (redir_cur);
}
