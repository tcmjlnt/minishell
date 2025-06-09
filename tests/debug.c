/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:28:58 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/08 11:34:50 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	debug_env(t_shell *shell)
{
	ft_printf("📦 DEBUG: Liste t_env (copie interne du shell)\n");
	print_env(shell->env);
}

void	debug_env_tab(t_shell *shell)
{
	char	**env_tab;

	env_tab = env_to_env_tab_for_execve(shell->env);
	ft_printf("📦 DEBUG: Tableau env_tab pour execve\n");
	print_env_tab(env_tab);
}

void	debug_path_for(char *cmd, t_shell *shell)
{
	ft_printf("📦 DEBUG: Recherche du path pour cmd : %s\n", cmd);
	print_cmd_path_found(cmd, shell->env);
}

void	debug_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (!cmd)
	{
		printf("[CMD DEBUG] NULL command\n");
		return ;
	}
	printf("=== Command Debug ===\n");
	printf("cmd->cmd: %s\n", cmd->cmd);
	if (cmd->args)
	{
		while (cmd->args[i])
		{
			printf("args[%d]: %s\n", i, cmd->args[i]);
			i++;
		}
	}
	printf("is_builtin: %u\n", cmd->is_builtin);
	printf("pipe[0]: %d | pipe[1]: %d\n", cmd->pipe[0], cmd->pipe[1]);
	printf("======================\n");
}

void	debug_cmd_list(t_cmd *cmds)
{
	while (cmds)
	{
		debug_cmd(cmds);
		cmds = cmds->next;
	}
}
void	debug_pipe(int in_fd, int pipe_fd[2])
{
	printf("[PIPE DEBUG] in_fd: %d | pipe_fd[0]: %d | pipe_fd[1]: %d\n", in_fd, pipe_fd[0], pipe_fd[1]);
}

void	debug_exec_external(t_cmd *cmd)
{
	int i = 0;
	printf("[EXTERNAL EXEC] Running: %s\n", cmd->cmd);
	while (cmd->args[i])
	{
		printf("arg[%d]: %s\n", i, cmd->args[i]);
		i++;
	}
}
