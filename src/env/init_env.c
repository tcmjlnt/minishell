/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:56:02 by aumartin          #+#    #+#             */
/*   Updated: 2025/05/13 11:47:17 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*env_new(char *key, char *value, t_bool equal)
{
	t_env	*new;

	new = gc_mem(GC_ALLOC, sizeof(t_env), NULL, GC_ENV);
	if (!new)
		return (NULL);
	new->key = key;
	new->value = value;
	new->equal = equal;
	new->next = NULL;
	return (new);
}

void	env_add_back(t_env **lst, t_env *new)
{
	t_env	*tmp;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	env_init(t_shell *shell, char **envp)
{
	int		i;
	char	*equal;
	char	*key;
	char	*val;
	t_env	*new;
	size_t	key_len;

	shell->env = NULL;
	i = 0;
	while (envp[i])
	{
		equal = ft_strchr(envp[i], '=');
		if (equal)
		{
			key_len = equal - envp[i];
			key = gc_mem(GC_ALLOC, key_len + 1, NULL, GC_ENV);
			if (!key)
				error_exit("malloc key");
			ft_strlcpy(key, envp[i], key_len + 1);
			val = gc_mem(GC_ALLOC, ft_strlen(equal + 1) + 1, NULL, GC_ENV);
			if (!val)
				error_exit("malloc val");
			ft_strlcpy(val, equal + 1, ft_strlen(equal + 1) + 1);
			new = env_new(key, val, true);
		}
		else
		{
			key = gc_mem(GC_ALLOC, ft_strlen(envp[i]) + 1, NULL, GC_ENV);
			if (!key)
				error_exit("malloc key no equal");
			ft_strlcpy(key, envp[i], ft_strlen(envp[i]) + 1);
			new = env_new(key, NULL, false);
		}
		env_add_back(&shell->env, new);
		i++;
	}
}

char	*get_env_value(t_env *env, const char *key)
{
	if (!key)
		return (NULL);
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0 && env->equal == true)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
