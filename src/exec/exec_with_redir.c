/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_with_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 15:31:32 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/20 09:01:55 by aumartin         ###   ########.fr       */
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

void	exec_external_cmd(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	if (is_directory(cmd->cmd))
	{
		ft_printf("minishell: %s: Is a directory\n", cmd->cmd);
		exit(126);
	}
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
	pid_t			pid;
	t_std_backup	std_backup;

	if (cmd->is_builtin)
	{
		save_std(&std_backup);
		if (apply_redirections(cmd, shell) == -1)
		{
			shell->exit_status = 1;
			close_redirections(cmd);
			restore_std(&std_backup);
			return ;
		}
		apply_dup_redirections(cmd);
		shell->exit_status = handle_builtin(shell, cmd, STDOUT_FILENO);
		close_redirections(cmd);
		restore_std(&std_backup);
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
/* 		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->exit_status = 128 + WTERMSIG(status); */
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
	while (current)
	{
		if (is_parent_builtin(current))
		{
			print_builtin_pipe_warning(current);
			shell->exit_status = 1;
			return ;
		}
		current = current->next;
	}
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

t_bool	is_valid_command(t_cmd *cmd, t_shell *shell)
{
	if (!cmd || !cmd->cmd || cmd->cmd[0] == '\0')
		return (false);
	if (is_directory(cmd->cmd))
		return (false);
	if (!cmd->is_builtin && !find_command_path(cmd->cmd, shell->env))
		return (false);
	return (true);
}

t_bool	check_invalid_cmds(t_cmd *cmd, t_shell *shell)
{
	t_cmd *current = cmd;

	while (current)
	{
		if (!current->is_builtin && !is_valid_command(current, shell))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			if (current->cmd)
				ft_putstr_fd(current->cmd, STDERR_FILENO);
			else
				ft_putstr_fd("(null)", STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			shell->exit_status = 127;
			return (true); // une erreur → on stop
		}
		current = current->next;
	}
	return (false); // tout va bien
}

void	exec_dispatcher(t_cmd *cmd, t_shell *shell)
{
	if (!cmd)
		return ;
	if (check_invalid_cmds(cmd, shell))
		return ;
	if (!check_all_heredocs(cmd, shell))
		return ;
	if (!cmd->next)
		exec_single_cmd(cmd, shell);
	else
		exec_pipeline(cmd, shell);
}
