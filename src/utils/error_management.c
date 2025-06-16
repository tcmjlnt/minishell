/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 18:33:46 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/14 16:41:17 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	error_exit(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}
// a implemnter ??
int	get_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return 1;
}

void	print_builtin_pipe_warning(t_cmd *cmd)
{
	int	i;

	i = 0;
	ft_putstr_fd("minishell: unsupported: builtin in pipeline\n", STDERR_FILENO);
	ft_putstr_fd("👉 Le builtin `", STDERR_FILENO);
	ft_putstr_fd(cmd->args[0], STDERR_FILENO);
	ft_putstr_fd("` ne peut pas être utilisé dans un pipeline car il affecte uniquement le shell principal.\n", STDERR_FILENO);
	ft_putstr_fd("   Exemple de commande incorrecte :\n   → ", STDERR_FILENO);
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], STDERR_FILENO);
		if (cmd->args[i + 1])
			ft_putstr_fd(" ", STDERR_FILENO);
		i++;
	}
	ft_putstr_fd(" | ...\n", STDERR_FILENO);
	ft_putstr_fd("💡 Utilise plutôt deux commandes séparées :\n", STDERR_FILENO);
	ft_putstr_fd("      ", STDERR_FILENO);
	i = 0;
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], STDERR_FILENO);
		if (cmd->args[i + 1])
			ft_putstr_fd(" ", STDERR_FILENO);
		i++;
	}
	ft_putstr_fd("\n      autre_commande\n", STDERR_FILENO);
}
