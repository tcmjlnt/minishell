/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:50:16 by aumartin          #+#    #+#             */
/*   Updated: 2025/05/07 12:21:02 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Initialiser les structures globales (environnement, garbage collector, etc.)
// Gérer la boucle principale du shell : affichage du prompt, lecture ligne,
// parsing, exécution
// Libérer toutes les ressources à la fin (gc_free_all avec GC_NONE)

#include "include/minishell.h"

int main(int ac, char **av, char **envp)
{
	t_shell	shell;
	(void)ac;
	(void)av;

	write(1, "Let's goooooo\n", 15); // a sup
	ft_prompt();

	init_shell();

	shell.envp = envp;

	char *path_env = getenv("PATH");
	char *path;
	char full_path[1024];
	char *path_copy;
	if (path_env == NULL)
		return (NULL);
	path_copy = ft_strdup(path_env);
	
}
