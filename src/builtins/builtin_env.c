/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:37:06 by aumartin          #+#    #+#             */
/*   Updated: 2025/05/28 11:38:40 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Implémenter la commande builtin `env`
// Afficher toutes les variables d’environnement actuelles

#include "../../include/minishell.h"

void	ft_env(t_shell *shell)
{
	t_env	*curr;

	curr = shell->env;
	while (curr)
	{
		if (curr->value)
			printf("%s=%s\n", curr->key, curr->value);
		else if (curr->equal && !curr->value)
			printf("%s=\n", curr->key);
		curr = curr->next;
	}
	shell->exit_status = 0;
}
