/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_access.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 10:22:26 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/21 16:56:51 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Mettre a jour une variable d'environnement dans t_env */
void	update_env_value(t_env *env, const char *key, const char *new_value)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			gc_mem(GC_FREE_ONE, 0, env->value, GC_ENV);
			// free(env->value); // a supprimer potentiellement a ne pas faire la mais GC
			env->equal = true;
			env->value = gc_strdup(new_value, GC_ENV);
			return ;
		}
		env = env->next;
	}
}


char	*get_env_value(t_env *env, const char *key, t_gc_type type)
{
	char	*res;

	res = NULL;
	if (!key)
		return (NULL);
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0 && env->equal == true) //check si a garder apres implementatin , export ?
		{
			res = env->value;
			if (!res)
				return (NULL);
			return (res);
		}
		env = env->next;
	}
	res = gc_strdup("", type); // hmm jutilise cette fonction dans la creation des xpd, est-ce que le type c'est GC_ENV ou GC_TKN ?
	if (!res)
		return (NULL);
	return (res);
}


// char	*get_env_value2(t_env *env, const char *key) // verifier utilisation dans exec
// {
// 	if (!key)
// 		return (NULL);
// 	while (env)
// 	{
// 		if (ft_strcmp(env->key, key) == 0 && env->equal == true) //check si a garder apres implementatin , export ?
// 			return (env->value);
// 		env = env->next;
// 	}
// 	return (NULL);
// }
