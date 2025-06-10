/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:50:16 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/10 13:56:20 by aumartin         ###   ########.fr       */
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
	write(1, "Let's TEST\n\n\n", 14);
	init_shell();
	shell = get_shell();
	env_from_envp(shell, envp);
	t_cmd *cmd = gc_mem(GC_ALLOC, sizeof(t_cmd), NULL, GC_CMD);
	ft_memset(cmd, 0, sizeof(t_cmd));

	//test_builtin_pwd(shell, cmd);
	//test_builtin_env(shell, cmd);
	//test_builtin_echo(shell, cmd);
	//test_handle_builtin(shell, cmd);

	// test_exec_cmd(shell, cmd);
	// run_all_tests(shell);
	run_redirections_tests(shell);
	gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);

	return (0);
}
