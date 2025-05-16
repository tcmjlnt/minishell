/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:10:54 by aumartin          #+#    #+#             */
/*   Updated: 2025/05/16 10:57:29 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// SI CHEMIN ABSOLU OU RELATIF DONNE ??? AJOUTER CE CAS
char	*find_command_path(char *cmd, t_env *env)
{
	char	*path_env;
	char	**paths;
	char	full_path[1024];
	int		i;

	path_env = get_env_value(env, "PATH");
	if (!path_env)
		return (NULL);
	paths = gc_split(path_env, ':', GC_CMD);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		ft_strlcpy(full_path, paths[i], sizeof(full_path));
		ft_strlcat(full_path, "/", sizeof(full_path));
		ft_strlcat(full_path, cmd, sizeof(full_path));
		if (access(full_path, X_OK) == 0)
			return (gc_strdup(full_path, GC_CMD));
		i++;
	}
	return (NULL);
}

/* while (x cmdes a faire)
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
			free(cmd_path);
			exit(EXIT_FAILURE);
		}

		close(in_fd);
		close(out_fd);

		char *cmd_path = find_cmd_path(cmd[0], env);
		if (!cmd_path)
		{
			perror("cmd not found");
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
 */
