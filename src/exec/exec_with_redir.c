/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_with_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 15:31:32 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/09 16:40:17 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Attention applique les redir AVANT exec du builtin

/* single commande sans pipe */
void	exec_single_cmd(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;

	if (cmd->is_builtin)
	{
		if (apply_redirections(cmd) == -1)
		{
			shell->exit_status = 1;
			close_redirections(cmd);
			return ;
		}
		apply_dup_redirections(cmd);
		shell->exit_status = handle_builtin(shell, cmd, STDOUT_FILENO);
		close_redirections(cmd);
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			prepare_child(cmd, shell);
			exec_external_cmd(cmd, shell);
			exit(shell->exit_status);
		}
		waitpid(pid, NULL, 0);
	}
}

/* handle pipeline */
void	exec_first_child(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;

	pipe_create(cmd->pipe);
	pid = fork();
	if (pid == 0)
	{
		dup2(cmd->pipe[1], STDOUT_FILENO);
		close(cmd->pipe[1]);
		prepare_child(cmd, shell);
		exec_external_cmd(cmd, shell);
		exit(shell->exit_status);
	}
	cmd->pid = pid;
	close(cmd->pipe[1]);
}

void	exec_middle_child(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;

	pipe_create(cmd->pipe);
	pid = fork();
	if (pid == 0)
	{
		dup2(cmd->prev->pipe[0], STDIN_FILENO);
		close(cmd->prev->pipe[0]);
		dup2(cmd->pipe[1], STDOUT_FILENO);
		close(cmd->pipe[1]);
		prepare_child(cmd, shell);
		exec_external_cmd(cmd, shell);
		exit(shell->exit_status);
	}
	cmd->pid = pid;
	close(cmd->prev->pipe[0]);
	close(cmd->pipe[1]);
}

void	exec_last_child(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		dup2(cmd->prev->pipe[0], STDIN_FILENO);
		close(cmd->prev->pipe[0]);
		prepare_child(cmd, shell);
		exec_external_cmd(cmd, shell);
		exit(shell->exit_status);
	}
	cmd->pid = pid;
	close(cmd->prev->pipe[0]);
}

void	exec_pipeline(t_cmd *cmd, t_shell *shell)
{
	t_cmd	*current;

	current = cmd;
	exec_first_child(current, shell);
	current = current->next;
	while (current->next)
	{
		exec_middle_child(current, shell);
		current = current->next;
	}
	exec_last_child(current, shell);
	current = cmd;
	while (current)
	{
		waitpid(current->pid, NULL, 0);
		current = current->next;
	}
}

void	exec_dispatcher(t_cmd *cmd, t_shell *shell)
{
	if (!cmd)
		return ;
	if (!cmd->next)
		exec_single_cmd(cmd, shell);
	else
		exec_pipeline(cmd, shell);
}
