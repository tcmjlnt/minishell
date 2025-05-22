/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:50:16 by aumartin          #+#    #+#             */
/*   Updated: 2025/05/22 15:57:36 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Initialiser les structures globales (environnement, garbage collector, etc.)
// Gérer la boucle principale du shell : affichage du prompt, lecture ligne,
// parsing, exécution
// Libérer toutes les ressources à la fin (gc_free_all avec GC_NONE)

#include "../include/minishell.h"
#include "tests.h"

int main(int ac, char **av, char **envp)
{
	t_shell	*shell;
	(void)ac;
	(void)av;

	write(1, "Let's goooooo\n\n\n", 15); // a sup

	// ft_prompt();

	init_shell();

	shell = get_shell();

	env_from_envp(shell, envp);
	// print_envp(envp);
	// print_env_line(envp[5]);
	// print_env(shell->env);
	// get_path_from_env(shell->env);

	// print_cmd_path_found(av[1], shell->env);

/* 	char **env_tab = env_to_env_tab_for_execve(shell->env);
	print_env_tab(env_tab); */

	// test_export_var2(shell);
	test_exec_echo();

	return (0);
}
