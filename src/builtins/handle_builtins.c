/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 08:57:44 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/04 13:42:37 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_builtin g_builtins[] = {
	{"pwd", ft_pwd},
	{"env", ft_env},
	{"echo", ft_echo},
	{NULL, NULL}
};

int	handle_builtin(t_shell *shell, t_cmd *cmd, int fd)
{
	int		i;
	char	**args;

	args = cmd->args;
	i = 0;
	if (!args || !args[0])
		return (0);
	while (g_builtins[i].name)
	{
		if (ft_strcmp(args[0], g_builtins[i].name) == 0)
			return (g_builtins[i].func(shell, cmd, fd));
		i++;
	}
	return (0);
}
