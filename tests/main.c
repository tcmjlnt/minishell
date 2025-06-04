/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:50:16 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/04 09:59:42 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Initialiser les structures globales (environnement, garbage collector, etc.)
// Gérer la boucle principale du shell : affichage du prompt, lecture ligne,
// parsing, exécution
// Libérer toutes les ressources à la fin (gc_free_all avec GC_NONE)

#include "tests.h"

int	main(int ac, char **av, char **envp)
{
	t_shell	*shell;

	(void)ac;
	(void)av;
	write(1, "Let's TEST\n\n\n", 15);
	init_shell();
	shell = get_shell();
	env_from_envp(shell, envp);
	shell->current_cmd = gc_mem(GC_ALLOC, sizeof(t_cmd), NULL, GC_CMD);
	ft_memset(shell->current_cmd, 0, sizeof(t_cmd));

	//test_builtin_pwd(shell);
	//test_builtin_env(shell);
	//test_builtin_echo(shell);
	test_handle_builtin(shell);
	return (0);
}
