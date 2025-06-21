/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:57:36 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/21 17:31:48 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* save les descripteurs originaux de STDIN/STDOUT */
void	save_std(t_std_backup *backup)
{
	backup->orig_stdin = dup(STDIN_FILENO);
	backup->orig_stdout = dup(STDOUT_FILENO);
	backup->orig_stderr = dup(STDERR_FILENO);
}

/* restaure les descripteurs STDIN/STDOUT depuis la sauvegarde */
void	restore_std(t_std_backup *backup)
{
	if (backup->orig_stdin != -1)
	{
		dup2(backup->orig_stdin, STDIN_FILENO);
		close(backup->orig_stdin);
	}
	if (backup->orig_stdout != -1)
	{
		dup2(backup->orig_stdout, STDOUT_FILENO);
		close(backup->orig_stdout);
	}
	if (backup->orig_stderr != -1)
	{
		dup2(backup->orig_stderr, STDERR_FILENO);
		close(backup->orig_stderr);
	}
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
			restore_std(&std_backup);
			return ;
		}
		shell->exit_status = handle_builtin(shell, cmd, STDOUT_FILENO);
		restore_std(&std_backup);
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			apply_redirections(cmd, shell);
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
