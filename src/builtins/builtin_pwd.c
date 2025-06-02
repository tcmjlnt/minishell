/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:52:10 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/02 12:12:17 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Afficher le chemin absolu courant

en bash :
pwd : Affiche le chemin absolu du répertoire courant.

bash utilise la variable $PWD, mais la vérifie avec getcwd() si besoin.

🧱 À implémenter
Utilise getcwd() directement.

Gère les erreurs si getcwd() échoue (ex : dossier supprimé). */

// pas gerer pour les redirections ???

#include "../../include/minishell.h"

void	ft_pwd(t_shell *shell)
{
	char	buf[PATH_MAX];

	if (getcwd(buf, PATH_MAX))
	{
		printf("%s\n", buf);
		shell->exit_status = 0;
	}
	else
	{
		perror("pwd");
		shell->exit_status = 1;
	}
}
