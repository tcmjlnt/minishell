/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 14:56:38 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/21 18:01:02 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	create_pipes(t_cmd *cmd)
{
	t_cmd	*command;

	command = cmd;
	while (command->next)
	{
		if (pipe(command->pipe) == -1)
		{
			close_all_pipes(command->prev);
			return (-1);
		}
			command = command->next;
	}
	return (0);
}

void	close_all_pipes(t_cmd *command)
{
	t_cmd	*tmp;

	tmp = command;
	while (tmp)
	{
		close(tmp->pipe[0]);
		close(tmp->pipe[1]);
		tmp = tmp->prev;
	}
}

void	wait_pipeline(t_cmd *cmds, t_shell *shell)
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
Fait un `dup2` pour `STDIN` depuis le pipe de gauche (`pipes[i-1][0]`).
Ne fait **pas** de `dup2` pour `STDOUT` (il garde la sortie standard du shell).

- Enfants du milieu** (`0 < i < cmd_count - 1`) :
Fait un `dup2` pour `STDIN` depuis le pipe de gauche (`pipes[i-1][0]`).
Fait un `dup2` pour `STDOUT` vers le pipe de droite (`pipes[i][1]`).
*/

void	pipeline_child_life(t_cmd *cmd, t_cmd *prev_cmd, t_shell *shell, t_cmd *cmd_list)
{
	// proteger les dup2
	if (prev_cmd && cmd->next)
	{
		dup2(prev_cmd->pipe[1], STDIN_FILENO);
		dup2(cmd->pipe[0], STDOUT_FILENO);
	}
	else if (cmd->next == NULL)
		dup2(prev_cmd->pipe[1], STDIN_FILENO);
	else if (prev_cmd == NULL)
		dup2(cmd->pipe[0], STDOUT_FILENO);
	close_all_pipes(cmd_list);
	if (apply_redirections(cmd, shell) == -1)
		ft_exit(shell, cmd_list, -99);
	check_invalid_cmds(cmd_list, shell);
	if (cmd->is_builtin)
	{
		shell->exit_status = handle_builtin(shell, cmd, STDOUT_FILENO);
		exit (shell->exit_status);
	}
	else
		exec_external_cmd(cmd, shell);
	exit(1);
}

void	exec_pipeline(t_cmd *cmd_list, t_shell *shell)
{
	int		cmd_count;
	t_cmd	*cmd;
	int		i;

	i = 0;
	cmd_count = 0;
	cmd = cmd_list;
	cmd_count = ft_lstsize((t_list *)cmd_list);
	create_pipes(cmd_list);
	if (create_pipes(cmd_list) == -1)
	{
		shell->exit_status = 1;
		ft_exit(shell, cmd_list, -99);
	}
	while (cmd)
	{
		cmd->pid = fork();
		if (cmd->pid == 0)
			pipeline_child_life(cmd, cmd->prev, shell, cmd_list);
		cmd = cmd->next;
		i++;
	}
	close_all_pipes(cmd_list);
	wait_pipeline(cmd_list, shell);
}
