/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:52:57 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/04 18:58:24 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_cmd	*create_cmd(char *cmd_name, char **args, int fd_in, int fd_out)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->cmd = cmd_name;
	cmd->args = args;
	cmd->fd_in = fd_in;
	cmd->fd_out = fd_out;
	cmd->is_builtin = is_builtin(cmd_name);
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
