/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:57:36 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/25 11:23:30 by aumartin         ###   ########.fr       */
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

t_bool	is_valid_command(t_cmd *cmd, t_shell *shell, int *status, char **path)
{
	if (!cmd->cmd)
		return (false);
	else if (cmd->cmd[0] == '\0')
	{
		ft_putstr_fd(cmd->cmd, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		*status = 127;
		return (false);
	}
	else if (is_directory(cmd->cmd))
	{
		ft_putstr_fd(cmd->cmd, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		*status = 126;
		return (false);
	}
	*path = find_command_path(cmd->cmd, shell->env);
	if (!cmd->is_builtin && *path == NULL)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->cmd, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		*status = 127;
		return (false);
	}
	return (true);
}

t_bool	is_directory(char *file)
{
	int	fd;

	fd = open(file, O_DIRECTORY);
	if (fd < 0)
		return (false);
	close(fd);
	return (true);
}

/* single commande sans pipe */
void	exec_single_cmd(t_cmd *cmd, t_shell *shell)
{
	pid_t			pid;
	t_std_backup	std_backup;
	int	exit_status;
	char	*path;

	exit_status = 0;
	path = NULL;
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
			if (apply_redirections(cmd, shell) == -1)
				exit (1);
			if (is_valid_command(cmd, shell, &exit_status, &path))
				execve(path, cmd->args, env_to_env_tab_for_execve(shell->env));
			exit(exit_status);
		}
		wait_for_children(cmd, shell);
	}
}
