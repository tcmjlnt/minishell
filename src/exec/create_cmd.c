/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:52:57 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/05 15:22:04 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_cmd	*create_cmd(char *cmd_name, char **args, int fd_in, int fd_out, t_shell *shell)
{
	t_cmd		*cmd;
	t_builtin	*builtins;

	builtins = shell->builtins;
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->cmd = cmd_name;
	cmd->args = args;
	cmd->fd_in = fd_in;
	cmd->fd_out = fd_out;
	cmd->is_builtin = is_builtin(shell, cmd_name);
	cmd->next = NULL;
	cmd->prev = NULL;
	return (cmd);
}


void	add_cmd(t_cmd **head, t_cmd *new_cmd)
{
	t_cmd	*current;

	if (!head || !new_cmd)
		return ;
	if (*head == NULL)
	{
		*head = new_cmd;
		return ;
	}
	current = *head;
	while (current->next != NULL)
		current = current->next;
	current->next = new_cmd;
	new_cmd->prev = current;
}
