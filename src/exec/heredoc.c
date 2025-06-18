/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 12:53:38 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/18 13:35:38 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// utiliser readline au lieu de gnl
// tout foutre dans un tmp ??
// a implenter apres signaux, voir Eric ??

int	here_doc(t_exec *exec, char *limiter, t_cmd *cmd, t_shell *shell)
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
		write_here_doc(pipe_fd[1], limiter);
		shell->exit_status = 0;
		exit(EXIT_SUCCESS);
	}
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		error_exit("child process failed");
	exec->in_fd = pipe_fd[0];
	exec_dispatcher(cmd, shell);
	close(pipe_fd[0]);
	return (shell->exit_status = 0);
}

void	write_here_doc(int fd, char *limiter)
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
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\n')
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
	close(fd);
}
