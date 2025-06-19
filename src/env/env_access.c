/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_access.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 10:22:26 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/19 11:33:22 by tjacquel         ###   ########.fr       */
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
			env->equal = true;
			env->value = gc_strdup(new_value, GC_ENV);
			return ;
		}
		env = env->next;
	}
}

char	*get_env_value(t_env *env, const char *key)
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
	res = ft_strdup("");
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
