/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 19:24:39 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/06 14:26:01 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* stocker les pid dans t_cmd ??

manip les processus après leur fork  ?? faire des kill(pid)
recup les exit_status ??

*/
static pid_t	*alloc_pids(int cmd_count)
{
	pid_t	*pids;

	pids = gc_mem(GC_ALLOC, sizeof(pid_t) * cmd_count, NULL, GC_CMD);
	if (!pids)
		error_exit("malloc pids");
	return (pids);
}

void	init_pipes(t_cmd *cmds)
{
	while (cmds)
	{
		if (cmds->next)
		{
			if (pipe(cmds->pipe) == -1)
				error_exit("pipe");
		}
		cmds = cmds->next;
	}
}

static void	exec_child(t_cmd *cmd, int in_fd, t_shell *shell)
{
	printf("in_fd = %d et out_fd = %d  et cmd->pipe[1]= %d \n\n", in_fd, cmd->fd_out, cmd->pipe[1]);
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (cmd->next)
	{
		dup2(cmd->pipe[1], STDOUT_FILENO);
		close(cmd->pipe[1]);
		close(cmd->pipe[0]);
	}
	else
	{
		printf("je suis la derniere commande ???\n");
	}
	if (cmd->is_builtin)
		handle_builtin(shell, cmd, STDOUT_FILENO);
	else
		exec_cmd(cmd, shell->env);
	exit(EXIT_SUCCESS);
}

static int	count_cmds(t_cmd *cmds)
{
	int	count;

	count = 0;
	while (cmds)
	{
		count++;
		cmds = cmds->next;
	}
	return (count);
}
static int	update_fds(int in_fd, t_cmd *cmd)
{
	if (in_fd != STDIN_FILENO)
	{
		printf("in_fd = %d != STDIN_FILENO = %d\n", in_fd, STDIN_FILENO);
		printf("je close le in_fd\n");
		close(in_fd);
	}
	if (cmd->next)
	{
		printf("j'ai une cmd suivante donc je close pipe[1] = %d\n", cmd->pipe[1]);
		close(cmd->pipe[1]);
		return (cmd->pipe[0]);
	}
	return (STDIN_FILENO);
}

static void	wait_children(pid_t *pids, int cmd_count)
{
	int	i;

	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
}

void	exec_pipes(t_cmd *cmds, t_shell *shell)
{
	int		in_fd;
	int		cmd_count;
	pid_t	*pids;
	int		i;

	in_fd = STDIN_FILENO;
	cmd_count = count_cmds(cmds);
	pids = alloc_pids(cmd_count);
	i = 0;
	while (cmds)
	{
		if (cmd_count == 1 && cmds->is_builtin)
		{
			handle_builtin(shell, cmds, STDOUT_FILENO);
			return ;
		}
		pids[i] = fork();
		if (pids[i] == 0)
			exec_child(cmds, in_fd, shell);
		in_fd = update_fds(in_fd, cmds);
		cmds = cmds->next;
		i++;
	}
	wait_children(pids, cmd_count);
}



/* // execution des pipes a retravailler
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
} */
