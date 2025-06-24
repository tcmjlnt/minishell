/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 14:56:38 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/24 21:31:57 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	close_all_pipes(t_cmd *command)
{
	t_cmd	*tmp;

	tmp = command;
	while (tmp->prev)
	{
		tmp = tmp->prev;
	}
	while (tmp->next)
	{
		close(tmp->pipe[0]);
		close(tmp->pipe[1]);
		tmp = tmp->next;
	}
}

int	create_pipes(t_cmd *cmd)
{
	t_cmd	*command;
	// int i = 0;

	command = cmd;
	while (command->next)
	{
		if (pipe(command->pipe) == -1)
		{
			close_all_pipes(cmd);
			return (-1);
		}
		//printf("Création du pipe [%d,%d] pour cmd %s | cmd %s\n", command->pipe[0], command->pipe[1], command->cmd, command->next->cmd);
		command = command->next;
		//	i++;

	}
	return (0);
}

void	wait_for_children(t_cmd *cmds, t_shell *shell)
{
	t_cmd	*current;
	int		status;

	status = 0;
	current = cmds;
	while (current)
	{
		waitpid(current->pid, &status, 0);
		current = current->next;
	}
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
}

/*
- Premier enfant** (`i == 0`) :
Ne fait **pas** de `dup2` pour `STDIN` (il garde l'entrée standard du shell).
Fait un `dup2` pour `STDOUT` vers le pipe de droite (`pipes[0][1]`).

- Dernier enfant** (`i == cmd_count - 1`) :
Fait un `dup2` pour `STDIN` depuis le pipe de gauche (`cmd->prev->pipes[0]`).
Ne fait **pas** de `dup2` pour `STDOUT` (il garde la sortie standard du shell).

- Enfants du milieu** (`0 < i < cmd_count - 1`) :
Fait un `dup2` pour `STDIN` depuis le pipe de gauche (`pipes[i-1][0]`).
Fait un `dup2` pour `STDOUT` vers le pipe de droite (`pipes[i][1]`).
*/

void	pipeline_child_life(t_cmd *cmd, t_shell *shell)
{
	int	exit_status;
	char	*path;

	exit_status = 0;
	path = NULL;
	// proteger les dup2
	if (cmd->prev && cmd->next)
	{
		dup2(cmd->prev->pipe[0], STDIN_FILENO);
		dup2(cmd->pipe[1], STDOUT_FILENO);
	}
	else if (cmd->next == NULL)
		dup2(cmd->prev->pipe[0], STDIN_FILENO);
	else if (cmd->prev == NULL)
		dup2(cmd->pipe[1], STDOUT_FILENO);
	close_all_pipes(cmd);
	if (apply_redirections(cmd, shell) == -1)
		exit (1);
	if (!cmd->is_builtin && is_valid_command(cmd, shell, &exit_status, &path))
		execve(path, cmd->args, env_to_env_tab_for_execve(shell->env));
	else if (cmd->is_builtin)
	{
		exit_status = handle_builtin(shell, cmd, STDOUT_FILENO);
		gc_mem(GC_FREE_ALL, 0, NULL, GC_CMD);
		exit (exit_status);
	}
	gc_mem(GC_FREE_ALL, 0, NULL, GC_CMD);
	exit(exit_status);
}

void	exec_pipeline(t_cmd *cmd_list, t_shell *shell)
{
	t_cmd	*cmd_curr;
	pid_t	pid;

	cmd_curr = cmd_list;
	if (create_pipes(cmd_list) == -1)
		exit(1);
	while (cmd_curr)
	{
		pid = fork();
		if (pid == 0)
			pipeline_child_life(cmd_curr, shell);
		cmd_curr = cmd_curr->next;
	}
	close_all_pipes(cmd_list);
	wait_for_children(cmd_list, shell);
}
