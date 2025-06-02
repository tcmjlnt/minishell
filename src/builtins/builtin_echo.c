/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:12:48 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/02 12:43:38 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Affiche tous les args + les sépare par des espaces + ajoute un \n à la fin.

en bash :
echo				-> (juste une ligne vide)
echo hello			-> hello
echo hello world	-> hello world

Parcours les arguments args[1] jusqu’à la fin.
Affiches séparés par des espaces.
Termines par un \n.
Mets à jour shell->exit_status = 0. */

// A FaIRE : Gérer option -n (sans retour à la ligne)
// bash accepte plusieurs n collés : echo -nnnn hello -> donne hello sans \n.

#include "../../include/minishell.h"

void	ft_echo(t_shell *shell, char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	printf("\n");
	shell->exit_status = 0;
}
