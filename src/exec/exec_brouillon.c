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



// version sans pipe

void	exec_cmds_sanspipes(t_cmd *cmds, t_env *env)
{
	pid_t	pid;
	int		status;
	int		in_fd;
	int		out_fd;

	if (!cmds)
		return ;
	if (cmds->next == NULL && cmds->is_builtin)
	{
		printf("exec_cmds: executing single builtin\n");
		handle_builtin(get_shell(), cmds, cmds->fd_out);
		return ;
	}
	in_fd = dup(STDIN_FILENO);
	out_fd = dup(STDOUT_FILENO);

	while (cmds)
	{
		if (cmds->is_builtin)
		{
			printf("exec_cmds: calling handle_builtin\n"); // a sup
			handle_builtin(get_shell(), cmds, cmds->fd_out);
		}
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

	dup2(in_fd, STDIN_FILENO);
	dup2(out_fd, STDOUT_FILENO);
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
