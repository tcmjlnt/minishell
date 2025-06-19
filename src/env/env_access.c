/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_access.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 10:22:26 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/16 10:23:49 by aumartin         ###   ########.fr       */
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
	if (!key)
		return (NULL);
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0 && env->equal == true) //check si a garder apres implementatin , export ?
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
