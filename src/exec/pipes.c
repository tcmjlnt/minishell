/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 14:56:38 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/26 11:56:28 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	close_all_pipes(t_cmd *command)
{
	t_cmd	*tmp;

	tmp = command;
	while (tmp->prev)
	{
		tmp = tmp->prev;
	}
	while (tmp->next)
	{
		close(tmp->pipe[0]);
		close(tmp->pipe[1]);
		tmp = tmp->next;
	}
}

static int	create_pipes(t_cmd *cmd)
{
	t_cmd	*command;

	command = cmd;
	while (command->next)
	{
		if (pipe(command->pipe) == -1)
		{
			close_all_pipes(cmd);
			return (-1);
		}
		command = command->next;
	}
	return (0);
}

/*
- Premier enfant** (`i == 0`) :
Ne fait **pas** de `dup2` pour `STDIN` (il garde l'entrée standard du shell).
Fait un `dup2` pour `STDOUT` vers le pipe de droite (`pipes[0][1]`).

- Dernier enfant** (`i == cmd_count - 1`) :
Fait un `dup2` pour `STDIN` depuis le pipe de gauche (`cmd->prev->pipes[0]`).
Ne fait **pas** de `dup2` pour `STDOUT` (il garde la sortie standard du shell).

- Enfants du milieu** (`0 < i < cmd_count - 1`) :
Fait un `dup2` pour `STDIN` depuis le pipe de gauche (`pipes[i-1][0]`).
Fait un `dup2` pour `STDOUT` vers le pipe de droite (`pipes[i][1]`).
*/

static void	apply_dup_pipeline(t_cmd *cmd)
{
	if (cmd->prev && cmd->next)
	{
		if (dup2(cmd->prev->pipe[0], STDIN_FILENO) == -1
			|| dup2(cmd->pipe[1], STDOUT_FILENO) == -1)
			error_free_GC_CMD("dup2 failed (middle cmd)");
	}
	else if (cmd->next == NULL)
	{
		if (dup2(cmd->prev->pipe[0], STDIN_FILENO) == -1)
			error_free_GC_CMD("dup2 failed (last cmd)");
	}
	else if (cmd->prev == NULL)
	{
		if (dup2(cmd->pipe[1], STDOUT_FILENO) == -1)
			error_free_GC_CMD("dup2 failed (first cmd)");
	}
}

void	pipeline_childhood(t_cmd *cmd, t_shell *shell)
{
	int		exit_status;
	char	*path;

	exit_status = 0;
	path = NULL;
	apply_dup_pipeline(cmd);
	close_all_pipes(cmd);
	if (apply_redirections(cmd) == -1)
		error_free_GC_CMD("apply redir in pipeline failed");
	if (!cmd->is_builtin && is_valid_command(cmd, shell, &exit_status, &path))
	{
		execve(path, cmd->args, env_to_env_tab_for_execve(shell->env));
		error_free_GC_CMD("execve failed");
	}
	else if (cmd->is_builtin)
	{
		exit_status = handle_builtin(shell, cmd, STDOUT_FILENO);
		gc_mem(GC_FREE_ALL, 0, NULL, GC_CMD);
		exit (exit_status);
	}
	gc_mem(GC_FREE_ALL, 0, NULL, GC_CMD);
	exit(exit_status);
}

void	exec_pipeline(t_cmd *cmd_list, t_shell *shell)
{
	t_cmd	*cmd_curr;
	pid_t	pid;

	cmd_curr = cmd_list;
	if (create_pipes(cmd_list) == -1)
		error_free_GC_CMD("create_pipes failed");
	while (cmd_curr)
	{
		pid = fork();
/* 		if (pid == -1) // check si verif a faire ou non avec clio ?
		{
			perror("fork failed");
			close_all_pipes(cmd_list);
			wait_for_children(cmd_list, shell);
			shell->exit_status = 1;
			return ;
		} */
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			pipeline_childhood(cmd_curr, shell);
		}
		cmd_curr = cmd_curr->next;
	}
	close_all_pipes(cmd_list);
	wait_for_children(cmd_list, shell);
	free_and_cleanup_heredocs(cmd_list); // check si c'est bon pour le unlink
}

/* void	exec_pipeline(t_cmd *cmd_list, t_shell *shell)
{
	t_cmd	*cmd_curr;

	cmd_curr = cmd_list;
	if (create_pipes(cmd_list) == -1)
	{
		shell->exit_status = 1;
		ft_exit(shell, cmd_list, -99);
	}
	while (cmd_curr)
	{
		cmd_curr->pid = fork();
		cmd_curr = cmd_curr->next;
	}
	cmd_curr = cmd_list;
	while (cmd_curr)
	{
		if (cmd_curr->pid == 0)
			pipeline_childhood(cmd_curr, shell, cmd_list);
		cmd_curr = cmd_curr->next;
	}
	close_all_pipes(cmd_list);
	wait_for_children(cmd_list, shell);
} */

/* 	if (cmd->prev && cmd->next)
	{
		dup2(prev_cmd->pipe[1], STDIN_FILENO);
		dup2(cmd->pipe[0], STDOUT_FILENO);
	}
	else if (cmd->next == NULL)
		dup2(prev_cmd->pipe[1], STDIN_FILENO);
	else if (prev_cmd == NULL)
		dup2(cmd->pipe[0], STDOUT_FILENO); */
