/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 08:57:44 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/08 15:14:48 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_builtin(t_shell *shell, t_cmd *cmd, int fd)
{
	int			i;
	char		**args;
	t_builtin	*builtins;

	args = cmd->args;
	builtins = shell->builtins;
	i = 0;
	if (!args || !args[0])
		return (0);
	while (builtins[i].name)
	{
		if (ft_strcmp(args[0], builtins[i].name) == 0)
		{
			printf("handle_builtin: calling %s\n", args[0]);
			return (builtins[i].func(shell, cmd, fd));
		}
		i++;
	}
	shell->exit_status = 0;
	return (shell->exit_status);
}

// a intreger au parsing ?? (dès que tu as le t_cmd)	is_builtin() ➔ remplir cmd->is_builtin
t_bool	is_builtin(t_shell *shell, char *cmd_name)
{
	int			i;
	t_builtin	*builtins;

	i = 0;
	builtins = shell->builtins;
	if (!cmd_name)
		return (false);
	while (builtins[i].name)
	{
		if (ft_strcmp(cmd_name, builtins[i].name) == 0)
			return (true);
		i++;
	}
	return (false);
}
