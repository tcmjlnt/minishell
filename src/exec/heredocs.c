/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 12:53:38 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/25 11:34:14 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*gen_tmp_filename(void)
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

/* int	handle_heredoc(t_redir *redir)
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
		return (perror("open heredoc temp"), free(redir->file), -1); //GC
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
			break ;
		if (ft_strncmp(line, redir->delim, len) == 0 && line[len] == '\0')
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
} */



/* int	handle_heredoc(t_redir *redir)
{
	pid_t	pid; //pid process enfant pour heredoc
	int		status; //code retour de waitpid(status du child)
	int		fd; //fd du fichier temp dans lequel ecrit le heredoc

	//generer un nom de fichier temp pour ce heredoc
	redir->file = gen_tmp_filename();
	if (!redir->file)
		return (perror("malloc name"), -1);
	//ici on va cree un process enfant pour isoler la lecture du heredoc
	pid = fork();
	if (pid < 0)
		return (perror("fork"), free(redir->file), -1);
	// PROCESS ENFANT
	if (pid == 0)
	{
		//install le handler special heredoc sur SIGINT(ctrl+C), pour fermer stdin
		signal(SIGINT, signal_handler_heredoc);
		//ignorer SIGQUIT (^\, évite "Quit (core dumped)" dans heredoc)
		signal(SIGQUIT, SIG_IGN);
		//ouvrir le fichier temp en ecriture /troncature
		fd = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0600);
		if (fd == -1)
			exit(1);
		while (1) //lecture ligne a ligne
		{
			//affiche le prompt "heredoc> " et lit une ligne utilisateur
			char *line = readline("heredoc> ");
			if (!line)
				break; //sortie: ctrl+D ou stdin ferme via SIGINYT
			//si la ligne lue est exactement le delimiteur alors: fin du heredoc
			if (ft_strncmp(line, redir->delim, ft_strlen(redir->delim)) == 0 && line[ft_strlen(redir->delim)] == '\0')
			{
				free(line);
				break;
			}
			//sinon, ecrire la ligne dans le fichier temp
			ft_putendl_fd(line, fd);
			free(line);
		}
		close(fd); //fermer fichier temp
		exit(0); //fin process heredoc (successss)
	}
	//PROCESS PARENT (= SHELL PRINCIPAL)
	//ignore SIGINT pendant que l'enfant lit le heredoc
	//eviter de kill le shell si le user fait ctrl+c (car seule le child est concerne)
	signal(SIGINT, SIG_IGN);
	//attend la fin du process enfant heredoc
	waitpid(pid, &status, 0);
	//reinstall la gestion des signaux du shell parent cad prompt propre, ctrl+c reactive
	init_signals();
	//si l'enfant a ete killed par un SIGINT alors indiquer interruption
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (-1);
	//sinon tout est oki: heredoc ready, cmd peut etre lancee
	return (0);
} */





int	handle_heredoc_child(t_redir *redir)
{
	char	*line;
	int		fd;
	size_t	len;

	len = ft_strlen(redir->delim);
	redir->file = gen_tmp_filename();
	if (!redir->file)
	{
		perror("gen_tmp_filename");
		return (1);
	}
	fd = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("open heredoc temp");
		// gc_mem(GC_FREE_ALL, 0, NULL, GC_TMP); reteste avec val si tu dois le faire maintenant que c'est isoler
		return (1);
	}
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
			break ;
		if (ft_strncmp(line, redir->delim, len) == 0 && line[len] == '\0')
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	// gc_mem(GC_FREE_ALL, 0, NULL, GC_TMP); reteste avec val si tu dois le faire maintenant que c'est isoler
	return (0);
}

int	handle_heredoc(t_redir *redir, int *exit_status)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork heredoc");
		return (1);
	}
	else if (pid == 0)
	{
		// surement signaux par defaut ici
/* 		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL); */
		exit(handle_heredoc_child(redir)); // enfant sort avec le code d'erreur heredoc
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
		{
			*exit_status = 128 + WTERMSIG(status);
			write(1, "\n", 1); // check avec Clio si je dois utiliser ca ou autre chose
			return (1);
		}
		else if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			*exit_status = WEXITSTATUS(status);
			return (1);
		}
	}
	return (0);
}

int	handle_all_heredocs(t_cmd *cmd_list, int *exit_status, t_shell *shell)
{
	t_cmd	*cmd;
	t_redir	*redir;
	char	*path;
	int		res;

	res = 0;
	cmd = cmd_list;
	while (cmd)
	{
		redir = cmd->redir;
		while (redir)
		{
			if (redir->type == TOKEN_REDIRECT_HEREDOC)
			{
				if (handle_heredoc(redir, exit_status) != 0)
					return (1); // si interompt ou erreur
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	cmd = cmd_list;
	while (cmd)
	{
		if (!is_valid_command(cmd, shell, exit_status, &path))
			res = 1;
			//return (1);
		cmd = cmd->next;
	}
	return (res);
}
