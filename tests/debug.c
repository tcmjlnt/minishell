/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:28:58 by aumartin          #+#    #+#             */
/*   Updated: 2025/05/16 16:38:50 by aumartin         ###   ########.fr       */
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
	ft_printf("📦 DEBUG: Tableau env_tab (pour execve)\n");
	print_env_tab(env_tab);
}

void	debug_path_for(char *cmd, t_shell *shell)
{
	ft_printf("🔍 Recherche du path pour commande : %s\n", cmd);
	print_cmd_path_found(cmd, shell->env);
}
