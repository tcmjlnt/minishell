/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_build.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:56:02 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/26 17:57:10 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* creation d'un nouvel element de env avec GC */
t_env	*env_new(char *key, char *value, t_bool equal)
{
	t_env	*new;

	new = gc_mem(GC_ALLOC, sizeof(t_env), NULL, GC_ENV);
	if (!new)
		error_exit("env_new: allocation failed");
	new->key = key;
	new->value = value;
	new->equal = equal;
	new->next = NULL;
	return (new);
}

/* Ajouter un element en fin de liste chainee */
void	env_add_back(t_env **lst, t_env *new)
{
	t_env	*tmp;

	if (!lst || !new) // check avec thomas ?
		return ; // check avec thomas ?
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

/* parser une ligne envp ("KEY=VALUE") en élément t_env */
static t_env	*parse_env_line(char *line)
{
	char	*equal;
	char	*key;
	char	*val;
	size_t	key_len;

	equal = ft_strchr(line, '=');
	if (equal)
	{
		key_len = equal - line;
		key = gc_mem(GC_ALLOC, key_len + 1, NULL, GC_ENV);
		val = gc_mem(GC_ALLOC, ft_strlen(equal + 1) + 1, NULL, GC_ENV);
		if (!key || !val)
			error_exit("alloc key ou value");
		ft_strlcpy(key, line, key_len + 1);
		ft_strlcpy(val, equal + 1, ft_strlen(equal + 1) + 1);
		return (env_new(key, val, true));
	}
	key = gc_mem(GC_ALLOC, ft_strlen(line) + 1, NULL, GC_ENV);
	if (!key)
		error_exit("alloc key (no equal)");
	ft_strlcpy(key, line, ft_strlen(line) + 1);
	return (env_new(key, NULL, false));
}

/* cnstruction de l'environnement a partir de envp */
void	env_from_envp(t_shell *shell, char **envp)
{
	int		i;
	t_env	*new;

	if (!envp || !*envp)
	{
		ft_putstr_fd(WARNING_ENV, STDERR_FILENO);
		shell->exit_status = 1;
		exit(1);
		return ;
	}
	shell->env = NULL;
	i = 0;
	while (envp[i])
	{
		new = parse_env_line(envp[i]);
		if (!new)
		{
			gc_mem(GC_FREE_ALL, 0, NULL, GC_ENV);
			error_exit("alloc parse env");
		}
		env_add_back(&shell->env, new);
		i++;
	}
}
