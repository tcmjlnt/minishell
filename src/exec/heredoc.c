/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 12:53:38 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/21 18:54:51 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// utiliser readline au lieu de gnl
// tout foutre dans un tmp ??
// a implenter apres signaux, voir Eric ??

/* int	here_doc(char *delim, t_cmd *cmd, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipe_fd) == -1)
	{
		return (error_exit("pipe"), shell->exit_status = 1, -1);
	}
	pid = fork();
	if (pid == -1)
		return (error_exit("fork"), shell->exit_status = 1, -1);
	if (pid == 0)
	{
		close(pipe_fd[0]);
		write_here_doc(pipe_fd[1], delim);
		shell->exit_status = 0;
		exit(EXIT_SUCCESS);
	}
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		error_exit("child process failed");
	cmd->fd_in = pipe_fd[0];
	exec_dispatcher(cmd, shell);
	close(pipe_fd[0]);
	return (shell->exit_status = 0);
}

void	write_here_doc(int fd, char *delim)
{
	char	*line;

	line = NULL;
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		// line = readline("> ");
		if (!line)
			error_exit("readline");
		if (ft_strncmp(line, delim, ft_strlen(delim)) == 0
			&& line[ft_strlen(delim)] == '\n')
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
	close(fd);
} */
/* Ctrl+C dans un heredoc */
void	sigint_heredoc(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

/* Lecture de toutes les lignes de l'utilisateur jusqu'au delim */
char	*heredoc_read_loop(const char *delim)
{
	char	*line;
	char	*content;
	size_t	lim_len;

	content = NULL;
	lim_len = ft_strlen(delim);
	while (1)
	{
		write(1, "heredoc ", 8); // a sup ces t pour y voir
		write(1, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strncmp(line, delim, lim_len) == 0 && line[lim_len] == '\n')
		{
			// printf("delim: '%s', line: '%s'\n", delim, line);
			break ;
		}
		content = gc_strjoin(content, line, GC_TMP);
	}
	return (content);
}

/* Gère le heredoc dans un processus enfant, écrit le contenu dans le pipe */
static int	create_heredoc_pipe(char *delim, int *pipe_fd)
{
	pid_t	pid;
	char	*content;

	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), -1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		signal(SIGINT, sigint_heredoc);
		content = heredoc_read_loop(delim);
		if (content)
			write(pipe_fd[1], content, ft_strlen(content));
		close(pipe_fd[1]);
		exit(EXIT_SUCCESS);
	}
	close(pipe_fd[1]);
	waitpid(pid, NULL, 0);
	return (0);
}

/*
Crée un pipe, lit le contenu via GNL, le remplit avec les données jusqu'au
delim. Retourne le descripteur à utiliser pour rediriger l'entrée.
Met à jour cmd->fd_in.
 */
int	here_doc(char *delim, t_cmd *cmd, t_shell *shell)
{
	int	pipe_fd[2];

	if (!delim || !cmd || !shell)
		return (-1);
	//  print_args(cmd);
/* 	if (!cmd->cmd)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `<<'\n", STDERR_FILENO);
		shell->exit_status = 2;
		return (-1);
	} */
	if (create_heredoc_pipe(delim, pipe_fd) == -1)
	{
		shell->exit_status = 1;
		return (-1);
	}
	if (cmd->fd_in != STDIN_FILENO)
		close(cmd->fd_in);
	cmd->fd_in = pipe_fd[0];
	gc_mem(GC_FREE_ALL, 0, NULL, GC_TMP);
	return (0);
}
