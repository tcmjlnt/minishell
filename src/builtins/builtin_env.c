/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:37:06 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/04 13:43:25 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Afficher toutes les variables d’environnement actuelles

en bash :
env
Sans args : affiche l’environnement courant (copie exportée).

env VAR1=value1 VAR2=value2 command args
Avec VAR=value + commande : crée un environnement temporaire pour cette commande.
Ne montre que les variables exportées.

Les variables créées via VAR=value sans export ne sont pas visibles dans env.

=> 	env affiche les variables de t_env où equal == true + exit_status */

#include "../../include/minishell.h"

int	ft_env(t_shell *shell, t_cmd *cmd, int fd)
{
	t_env	*curr;

	(void) cmd;
	curr = shell->env;
	while (curr)
	{
		if (curr->value)
		{
			ft_putstr_fd(curr->key, fd);
			ft_putstr_fd("=", fd);
			ft_putstr_fd(curr->value, fd);
			ft_putstr_fd("\n", fd);
		}
		else if (curr->equal && !curr->value)
		{
			ft_putstr_fd(curr->key, fd);
			ft_putstr_fd("=\n", fd);
		}
		curr = curr->next;
	}
	shell->exit_status = 0;
	return (0);
}
