/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:10:54 by aumartin          #+#    #+#             */
/*   Updated: 2025/05/22 16:27:09 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* void	exec_command(t_cmd *cmds, t_env *env, t_shell *shell)
{
	char	*command_path;
	char	**env_tab;
	pid_t	pid;
	int		status;

	env_tab = env_to_env_tab_for_execve(env);

	if (shell->is_cmd == true)
	{
		;
	}

	command_path = find_command_path(cmds->args[0], env_tab);
	if (command_path == NULL)
		handle_exec_errors(ENOENT, cmds->cmd);

	pid = fork();
	if (pid == 0)
	{
		if (dup2(cmds->input, STDIN_FILENO) == -1
			|| dup2(cmds->output, STDOUT_FILENO) == -1)
		{
			ft_printf("%s\n", command_path);
			perror("dup2");
			free(command_path);
			exit(EXIT_FAILURE);
		}
		close(cmds->input);
		close(cmds->output);
		if (execve(command_path, cmds->cmd, env_tab) == -1)
		{
			handle_exec_errors(errno, cmds->cmd);
			perror("execve");
			free(command_path);
			exit(EXIT_FAILURE);
		}
		free(command_path);
	}


	else
	{
		perror("fork");
		error_exit("fork");
	}
} */


/* t_cmd cmd = {
	.cmd = "echo",
	.args = (char *[]){"echo", "bonjour", NULL},
	.redir = NULL,
	.pipe = {-1, -1},
	.pid = 0,
	.is_builtin = false,
	.exit_code = 0,
	.next = NULL,
	.prev = NULL
} */


void	exec_cmd(t_cmd *cmd, t_env *env)
{
	char	*path;
	char	**env_tab;

	if (!cmd || !cmd->cmd)
		error_exit("exec_command_simple: commande vide");

	path = find_command_path(cmd->cmd, env);
	if (!path)
	{
		ft_printf("minishell: %s: command not found\n", cmd->cmd);
		exit(127);
	}
	env_tab = env_to_env_tab_for_execve(env);

	if (execve(path, cmd->args, env_tab) == -1)
	{
		perror("execve");
		exit(126);
	}
	free(path);
}

void	exec_cmds(t_cmd *cmds, t_env *env)
{
	pid_t	pid;
	int		status;
	int		in_fd;
	int		out_fd;

	if (!cmds)
		return ;

	in_fd = dup(STDIN_FILENO);
	out_fd = dup(STDOUT_FILENO);

	while (cmds)
	{
		if (cmds->is_builtin)
			exec_cmd(cmds, env);
		else
		{
			pid = fork();
			if (pid == 0)
				exec_cmd(cmds, env);
			else if (pid > 0)
				waitpid(pid, &status, 0);
			else
				perror("fork");
		}
		cmds = cmds->next;
	}

	close(in_fd);
	close(out_fd);
}

/* while (x cmdes a faire)
{
	pid = fork();
	if (pid == 0)
	{
		dup2(in_fd, STDIN_FILENO);
		dup2(out_fd, STDOUT_FILENO);

		if (dup2(in_fd, STDIN_FILENO) == -1
		|| dup2(out_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			free(cmd_path);
			exit(EXIT_FAILURE);
		}

		close(in_fd);
		close(out_fd);

		char *cmd_path = find_cmd_path(cmd[0], env);
		if (!cmd_path)
		{
			perror("cmd not found");
			exit(127);
		}

		execve(cmd_path, cmd, env);

		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		close(in_fd);
		close(out_fd);
		wait_processes();
		waitpid(pid, &status, 0);
	}
	else
	{
		perror("fork");
		error_exit("fork");
	}
	// free a faire
}
 */
