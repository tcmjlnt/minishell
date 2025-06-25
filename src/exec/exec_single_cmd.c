/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:57:36 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/25 22:39:46 by aumartin         ###   ########.fr       */
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
	close(backup->orig_stdin);
	close(backup->orig_stdout);
	close(backup->orig_stderr);
}

void	single_cmd_childhood(t_cmd *cmd, t_shell *shell)
{
	int		exit_status;
	char	*path;

	exit_status = 0;
	path = NULL;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (apply_redirections(cmd, shell) == -1)
		exit (1);
	if (is_valid_command(cmd, shell, &exit_status, &path))
		execve(path, cmd->args, env_to_env_tab_for_execve(shell->env));
	gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);
	exit(exit_status);
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
			single_cmd_childhood(cmd, shell);
		wait_for_children(cmd, shell);
	}
}
