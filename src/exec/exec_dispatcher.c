/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_dispatcher.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 13:45:46 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/24 23:13:09 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	exec_dispatcher(t_cmd *cmd, t_shell *shell)
{
	int	exit_status;

	if (!cmd || !shell)
		return (1);
	exit_status = 0;
	if (handle_all_heredocs(cmd, &exit_status, shell) != 0)
	{
		shell->exit_status = exit_status;
		return (1); // pk tu ne veux pas continuer si signal ou heredoc fail
	}
	if (!cmd->next)
		exec_single_cmd(cmd, shell); // verifie si on fit bien remonter l'exit_status ou laors change fonction en type int
	else
		exec_pipeline(cmd, shell); // idem
	return (0);
}


/*
florent me dit de remettre le is_valid command avant essayer si tout se passe sinon integre le dans tes heredocs
	t_cmd	*tmp;
	tmp = cmd;
	while (tmp)
	{
		if (!is_valid_command(tmp, shell, &shell->exit_status, &path))
			return (1);
		tmp = tmp->next;
	}
*/

