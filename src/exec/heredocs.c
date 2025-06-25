/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 12:53:38 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/25 22:12:25 by tjacquel         ###   ########.fr       */
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

int	heredoc_childhood(t_redir *redir)
{
	char	*line;
	int		fd;
	size_t	len;

	fd = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
		exit(1);
	len = ft_strlen(redir->delim);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
		{
			if (g_sig == 0)
			{
				write(2, STDIN_EOF_WARNING, ft_strlen(STDIN_EOF_WARNING));
				write(2, STDIN_EOF_MSG, ft_strlen(STDIN_EOF_MSG));
				write(2, redir->delim, len);
				write(2, "')\n", 3);
				return (0);
				//return (write(2, "')\n", 3), parse->exit_code = EXIT_EOF, -1);
			}
			if (g_sig)
			{
				// printf("hellp\n");
				g_sig = 0;
				exit(SIGINT);
			}
		}
		if (ft_strncmp(line, redir->delim, len) == 0 && line[len] == '\0') // Clio a mis le strlen la
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	// gc_mem(GC_FREE_ALL, 0, NULL, GC_TMP); reteste avec val si tu dois le faire maintenant que c'est isoler
	return (0); // clio a mis exit (0)
}

/*
--> gere un heredoc pour une redirection de type <<
--> cree un fichier temp, lit ligne par ligne jusqu'au delimateur ou interruption
et ecrit le contenu dans ce fichier
*/
int	handle_heredoc(t_redir *redir)
{
	pid_t	pid;
	int		status;

	redir->file = gen_tmp_filename();
	if (!redir->file)
		return (perror("malloc name"), -1);
	// printf("%d\n",g_sig);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), free(redir->file), -1);
	if (pid == 0)
	{
		//install le handler special heredoc sur SIGINT(ctrl+C), pour fermer stdin
		//ignorer SIGQUIT (^\, évite "Quit (core dumped)" dans heredoc)
		signal(SIGINT, signal_handler_heredoc);
		signal(SIGQUIT, SIG_IGN);
		heredoc_childhood(redir);
		exit (0); // clio a mis le exit(0) direcement ici mais ca devirat etre pareil
		// moi j'avais mis mais ca deavrait etre pareil
		// exit(handle_heredoc_child(redir)); // enfant sort avec le code d'erreur heredoc
	}
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	//reinstall la gestion des signaux du shell parent cad prompt propre, ctrl+c reactive
	init_signals();

	//si l'enfant a ete killed par un SIGINT alors indiquer interruption
	// printf("signal %d \n", status);
	if (status)
		return (-1);
	//sinon tout est oki: heredoc ready, cmd peut etre lancee

	return (0);
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
			if (redir->type == TKN_HEREDOC)
			{
				if (handle_heredoc(redir) == -1)
				{
					// g_sig = SIGINT;
					return (-1);
				}
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
