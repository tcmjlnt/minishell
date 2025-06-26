/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_tab_build.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 13:16:06 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/27 01:25:10 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* compter les elements de l'env valides (avec '=') */
static int	count_env_tab(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->equal == true)
			count++;
		env = env->next;
	}
	return (count);
}

/* transformer un element env en string "KEY=VALUE" pour remplir le tab
 * 🚩 Pourquoi GC_CMD ici ?
 * → Parce que c’est utilisé uniquement pour execve (cmd exec)
 * → La mémoire sera libérée après l'exécution de la commande.
 */
static char	*fill_env_tab(t_env *entry)
{
	char	*line;
	size_t	len;

	len = ft_strlen(entry->key) + 1 + ft_strlen(entry->value) + 1;
	line = gc_mem(GC_ALLOC, len, NULL, GC_CMD); // GC_CMD ou GC_ENV ?
	if (!line)
		error_exit("fill_env_tab: allocation failed"); // faut free ? malloc fail error_free_gc()
	ft_strlcpy(line, entry->key, len);
	ft_strlcat(line, "=", len);
	ft_strlcat(line, entry->value, len);
	return (line);
}

/* construction du char ** depuis t_env pour execve */
char	**env_to_env_tab_for_execve(t_env *env)
{
	int		i;
	int		count;
	char	**env_tab;

	count = count_env_tab(env);
	env_tab = gc_mem(GC_ALLOC, sizeof(char *) * (count + 1), NULL, GC_CMD); // GC_CMD ou GC_ENV ?
	if (!env_tab)
		error_exit("env_to_env_tab_for_execve: alloc failed"); // faut free ? malloc fail error_free_gc()
	i = 0;
	while (env)
	{
		if (env->equal == true)
			env_tab[i++] = fill_env_tab(env);
		env = env->next;
	}
	env_tab[i] = NULL;
	return (env_tab);
}
