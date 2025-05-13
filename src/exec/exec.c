/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:10:54 by aumartin          #+#    #+#             */
/*   Updated: 2025/05/07 11:25:37 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Étape 1 : Préparer les entrées/sorties (pipes, redirs, etc.)
// in_fd / out_fd sont supposés déjà configurés

#include "../../include/minishell.h"

while (x commandes a faire)
{
	pid = fork();
	if (pid == 0)
	{
		dup2(in_fd, STDIN_FILENO);
		dup2(out_fd, STDOUT_FILENO);

		if (dup2(in_fd, STDIN_FILENO) == -1
		|| dup2(out_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			free(command_path);
			exit(EXIT_FAILURE);
		}

		close(in_fd);
		close(out_fd);

		char *cmd_path = find_command_path(cmd[0], env);
		if (!cmd_path)
		{
			perror("command not found");
			exit(127);
		}

		execve(cmd_path, cmd, env);

		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		close(in_fd);
		close(out_fd);
		wait_processes();
		waitpid(pid, &status, 0);
	}
	else
	{
		perror("fork");
		error_exit("fork");
	}
	// free a faire
}
