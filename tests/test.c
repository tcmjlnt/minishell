/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:38:06 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/04 09:26:20 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"

void	test_export_var1(t_shell *shell)
{
	t_env	*new;

	new = gc_mem(GC_ALLOC, sizeof(t_env), NULL, GC_ENV);
	new->key = gc_strdup("VAR1", GC_ENV);
	new->value = NULL;
	new->equal = false;
	new->next = shell->env;
	shell->env = new;
	ft_printf("✅ VAR1 exporté sans valeur (equal=false)\n");
	ft_printf("\nenv_tab (pour execve)\n");
	print_env_tab(env_to_env_tab_for_execve(shell->env));
}

void	test_export_var2(t_shell *shell)
{
	t_env	*env;
	t_env	*new;

	env = shell->env;
	new = gc_mem(GC_ALLOC, sizeof(t_env), NULL, GC_ENV);
	new->key = gc_strdup("VAR2", GC_ENV);
	new->value = gc_strdup("bonjour", GC_ENV);
	new->equal = true;
	new->next = env;
	shell->env = new;
	ft_printf("\n🔍 Contenu de t_env après export VAR2=bonjour :\n");
	print_env(shell->env);
	ft_printf("\nDEBUG : env_tab (pour execve)\n");
	print_env_tab(env_to_env_tab_for_execve(shell->env));
}

