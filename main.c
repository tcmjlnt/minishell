/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:50:16 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/25 16:44:20 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Initialiser les structures globales (environnement, garbage collector, etc.)
// Gérer la boucle principale du shell : affichage du prompt, lecture ligne,
// parsing, exécution
// Libérer toutes les ressources à la fin (gc_free_all avec GC_NONE)

#include "include/minishell.h"

int	g_sig = 0;

int main(int ac, char **av, char **envp)
{
	t_shell	*shell;
	int		res;
	(void)av;

	if (ac != 1)
		return (ft_putstr_fd("minsihell error, try ./minishell\n", 2), -1);

	// write(1, "Let's goooooo\n\n\n", 15); // a sup

	// if (ac == 1)
	// {

	init_shell();
	shell = get_shell();

	env_from_envp(shell, envp);
	// init_signals();
	ft_prompt(shell);
	res = shell->exit_status;
	gc_mem(GC_FREE_ALL, 0, NULL, GC_ENV);
	gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);
	// print_envp(envp);
	// print_env_line(envp[5]);
	// print_env(shell->env);
	// get_path_from_env(shell->env);

	// print_cmd_path_found(av[1], shell->env);

/* 	char **env_tab = env_to_env_tab_for_execve(shell->env);
	print_env_tab(env_tab); */

	// print_cmd_path_found(av[1], shell->env);
	// }
	// else
	// 	printf("Expected behavior: launch the executable without any other argument\n");
	return (res);
}
