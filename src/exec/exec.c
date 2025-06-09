/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:10:54 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/09 18:19:43 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

void	exec_single_cmd(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;

	if (cmd->is_builtin)
	{
		shell->exit_status = handle_builtin(shell, cmd, STDOUT_FILENO);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		exec_external_cmd(cmd, shell);
		exit(EXIT_FAILURE);
	}
	waitpid(pid, NULL, 0);
}

void	exec_first_child(t_cmd *cmd, t_exec *exec, t_shell *shell)
{
	exec->pid = fork();
	if (exec->pid == 0)
	{
		if (exec->pipe_fd[1] != -1)
		{
			dup2(exec->pipe_fd[1], STDOUT_FILENO);
			close(exec->pipe_fd[1]);
		}
		if (cmd->is_builtin)
			handle_builtin(shell, cmd, STDOUT_FILENO);
		else
			exec_external_cmd(cmd, shell);
		exit(EXIT_SUCCESS);
	}
	cmd->pid = exec->pid;
}

void	exec_middle_child(t_cmd *cmd, t_exec *exec, t_shell *shell)
{
	exec->pid = fork();
	if (exec->pid == 0)
	{
		dup2(exec->in_fd, STDIN_FILENO);
		if (exec->pipe_fd[1] != -1)
		{
			dup2(exec->pipe_fd[1], STDOUT_FILENO);
			close(exec->pipe_fd[1]);
		}
		if (cmd->is_builtin)
			handle_builtin(shell, cmd, STDOUT_FILENO);
		else
			exec_external_cmd(cmd, shell);
		exit(EXIT_SUCCESS);
	}
	cmd->pid = exec->pid;
}
void	exec_last_child(t_cmd *cmd, t_exec *exec, t_shell *shell)
{
	exec->pid = fork();
	if (exec->pid == 0)
	{
		dup2(exec->in_fd, STDIN_FILENO);
		if (cmd->is_builtin)
			handle_builtin(shell, cmd, STDOUT_FILENO);
		else
			exec_external_cmd(cmd, shell);
		exit(EXIT_SUCCESS);
	}
	cmd->pid = exec->pid;
}

void	exec_pipeline(t_cmd *cmds, t_shell *shell)
{
	t_cmd	*current;
	t_exec	exec;

	current = cmds;
	exec.in_fd = STDIN_FILENO;
	while (current)
	{
		if (current->next)
			pipe_create(exec.pipe_fd);
		else
			pipe_reset(exec.pipe_fd);
		if (current == cmds)
			exec_first_child(current, &exec, shell);
		else if (!current->next)
			exec_last_child(current, &exec, shell);
		else
			exec_middle_child(current, &exec, shell);
		parent_close_fds(&exec);
		exec.in_fd = exec.pipe_fd[0];
		current = current->next;
	}
	wait_pipeline(cmds);
}

void	exec_dispatcher(t_cmd *cmds, t_shell *shell)
{
	if (!cmds)
		return ;

	if (!cmds->next)
		exec_single_cmd(cmds, shell);
	else
		exec_pipeline(cmds, shell);
}
