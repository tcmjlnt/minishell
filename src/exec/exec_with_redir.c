/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_with_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 15:31:32 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/13 16:09:47 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Attention applique les redir AVANT exec du builtin

void	wait_pipeline(t_cmd *cmds)
{
	t_cmd	*current;

	current = cmds;
	while (current)
	{
		waitpid(current->pid, NULL, 0);
		current = current->next;
	}
}

void	pipe_create(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
		error_exit("pipe");
}

void	pipe_reset(int pipe_fd[2])
{
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
}

void	parent_close_fds(t_exec *exec)
{
	if (exec->in_fd != STDIN_FILENO)
		close(exec->in_fd);
	if (exec->pipe_fd[1] != -1)
		close(exec->pipe_fd[1]);
}

void	exec_external_cmd(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	path = find_command_path(cmd->cmd, shell->env);
	if (!path)
	{
		ft_printf("minishell: %s: command not found\n", cmd->cmd);
		exit(127);
	}
	execve(path, cmd->args, env_to_env_tab_for_execve(shell->env));
	perror("execve");
	exit(126);
}

/* single commande sans pipe */
void	exec_single_cmd(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;

	if (cmd->is_builtin)
	{
		if (apply_redirections(cmd, shell) == -1)
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
