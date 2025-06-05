/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 08:57:44 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/05 13:28:43 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* tableau d'objets pour tableau de func KEY VALEUR voir ds tests funcptr.c */

t_builtin	*get_builtins(void)
{
	t_builtin	*builtins;

	builtins = gc_mem(GC_ALLOC, sizeof(t_builtin) * 4, NULL, GC_CMD);
	if (!builtins)
		return (NULL);
	builtins[0] = (t_builtin){"pwd", ft_pwd};
	builtins[1] = (t_builtin){"env", ft_env};
	builtins[2] = (t_builtin){"echo", ft_echo};
	builtins[3] = (t_builtin){NULL, NULL};
	return (builtins);
}

int	handle_builtin(t_shell *shell, t_cmd *cmd, int fd)
{
	int			i;
	char		**args;
	t_builtin	*builtins;

	args = cmd->args;
	builtins = get_builtins();
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
	return (0);
}

// a intreger au parsing (dès que tu as le t_cmd)	is_builtin() ➔ remplir cmd->is_builtin
t_bool	is_builtin(char *cmd_name)
{
	int			i;
	t_builtin	*builtins;

	builtins = get_builtins();
	i = 0;
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
