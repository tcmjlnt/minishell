/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:50:16 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/27 16:45:49 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Initialiser les structures globales (environnement, garbage collector, etc.)
// Gérer la boucle principale du shell : affichage du prompt, lecture ligne,
// parsing, exécution
// Libérer toutes les ressources à la fin (gc_free_all avec GC_NONE)

#include "include/minishell.h"

int	g_sig = 0;

int	main(int ac, char **av, char **envp)
{
	t_shell	*shell;
	int		res;

	(void)av;

	if (ac != 1)
		return (ft_putstr_fd("minsihell error, try ./minishell\n", 2), -1);

	init_shell();
	shell = get_shell();

	env_from_envp(shell, envp);
	ft_prompt(shell);
	res = shell->exit_status;
	gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);

	return (res);
}
