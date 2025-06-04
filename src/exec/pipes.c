/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 19:24:39 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/04 20:26:32 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// stocker les pids
pid_t	*alloc_pids(int cmd_count)
{
	pid_t *pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
		error_exit("malloc pids");
	return (pids);
}

// execution des pipes
void	execute_pipes(t_cmd *cmds, t_env *env)
{
	int		pipe_fd[2];
	int		in_fd = STDIN_FILENO;
	pid_t	*pids;
	int		cmd_count = 0;
	t_cmd	*tmp = cmds;

	// combien de commandes -> ajouter un champ cmd_count a struct cmd ?
	while (tmp)
	{
		cmd_count++;
		tmp = tmp->next;
	}
	pids = alloc_pids(cmd_count);

	int i = 0;
	while (cmds)
	{
		if (cmds->next) // sauf derniere commande
			if (pipe(pipe_fd) == -1)
				error_exit("pipe");

		pids[i] = fork();
		if (pids[i] == 0)
		{
			// gestion des in
			if (in_fd != STDIN_FILENO)
			{
				dup2(in_fd, STDIN_FILENO);
				close(in_fd);
			}

			// gestion des in (sauf derniere commande)
			if (cmds->next)
			{
				dup2(pipe_fd[1], STDOUT_FILENO);
				close(pipe_fd[1]);
				close(pipe_fd[0]);
			}

			// exec commande
			char *path = find_command_path(cmds->cmd, env);
			if (!path)
			{
				ft_printf("minishell: %s: command not found\n", cmds->cmd);
				exit(127);
			}
			char **env_tab = env_to_env_tab_for_execve(env);
			execve(path, cmds->args, env_tab);
			perror("execve");
			exit(126);
		}

		// parent ferme ses fd inutiles
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (cmds->next)
		{
			close(pipe_fd[1]);
			in_fd = pipe_fd[0];
		}

		cmds = cmds->next;
		i++;
	}

	// parent attend tous les enfants
	for (int j = 0; j < cmd_count; j++)
		waitpid(pids[j], NULL, 0);

	free(pids);
}
