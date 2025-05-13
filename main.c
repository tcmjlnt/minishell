/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:50:16 by aumartin          #+#    #+#             */
/*   Updated: 2025/05/13 11:24:24 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Initialiser les structures globales (environnement, garbage collector, etc.)
// Gérer la boucle principale du shell : affichage du prompt, lecture ligne,
// parsing, exécution
// Libérer toutes les ressources à la fin (gc_free_all avec GC_NONE)

#include "include/minishell.h"

int main(int ac, char **av, char **envp)
{
	t_shell	*shell;
	(void)ac;
	(void)av;

	write(1, "Let's goooooo\n\n\n", 15); // a sup

	// ft_prompt();

	init_shell();

	shell = get_shell();

	env_init(shell, envp);

	print_env_init(shell->env);

	char *val = get_env_value(shell->env, "PATH");
	if (val)
		printf("PATH = %s\n", val);
	else
		printf("PATH non trouvé\n");


}
