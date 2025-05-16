/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:10:54 by aumartin          #+#    #+#             */
/*   Updated: 2025/05/16 09:36:29 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_path_from_env(t_env *env)
{
	return (get_env_value(env, "PATH"));
}

// SI CHEMIN ABSOLU OU RELATIF DONNE ??? AJOUTER CE CAS 

char	*find_command_path(char *command, t_env *env)
{
	char	*path_env;
	char	**paths;
	char	full_path[1024];
	int		i;

	path_env = get_path_from_env(env);
	if (!path_env)
		return (NULL);
	paths = split_path(path_env);
	if (!paths)
		return (perror("split_path"), NULL);

	i = 0;
	while (paths[i])
	{
		ft_strlcpy(full_path, paths[i], sizeof(full_path));
		ft_strlcat(full_path, "/", sizeof(full_path));
		ft_strlcat(full_path, command, sizeof(full_path));
		if (access(full_path, X_OK) == 0)
		{
			char *res = ft_strdup(full_path);
			free_strs(paths);
			return (res);
		}
		i++;
	}
	free_strs(paths);
	return (NULL);
}


/* while (x commandes a faire)
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
 */
