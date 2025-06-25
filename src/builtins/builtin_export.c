/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:15:31 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/24 18:42:54 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_env_export(t_env *env)
{
	while (env)
	{
		printf("export %s=", env->key);
		if (env->value)
			printf("\"%s\"", env->value);

		printf("\n");
		env = env->next;
	}
}

size_t check_key_export(char *arg)
{
	size_t	j;

	j = 0;

	while(arg[j] && arg[j + 1] && arg[j + 1] != '=')
	{
		if(!is_valid_keychar(arg[j]) || ft_isdigit(arg[0]) || arg[0] == '=')
		{
			// shell->exit_status = 1;
			write(2, "minishell: export: `", 20);
			write(2, arg, ft_strlen(arg));
			write(2, "': not a valid identifier\n", 26);
			return (0);
		}
		j++;
	}
	return (++j);
}

t_env	*check_existing_key(t_shell *shell, char *key)
{
	t_env	*env_curr;

	if (!shell->env)
		return (NULL);
	env_curr = shell->env;
	while(env_curr)
	{
		if (ft_strcmp(env_curr->key, key) == 0)
			return (env_curr);
		env_curr = env_curr->next;
	}
	return (NULL);
}

int	arg_to_env(t_shell *shell, char *arg, size_t n)
{
	(void) shell;
	char	*key;
	char	*value;
	t_env	*env_new_node;

	key = NULL;
	value = NULL;
	key = gc_strndup(arg, n, GC_ENV);
	if (!key)
		return (false);
	value = gc_strdup(arg + n + 1, GC_ENV);
	if (!value)
		return (false);
	env_new_node = check_existing_key(shell, key);
	if (env_new_node)
		update_env_value(shell->env, key, value);
	else
	{
		env_new_node = env_new(key, value, true);
		if (!env_new_node)
			return (false);
		env_add_back((t_env **)&shell->env, env_new_node);
	}
	return (true);
}

int	export_args(t_shell *shell, t_cmd *cmd)
{
	size_t	i;
	size_t	j;
	t_bool	res;

	i = 1;
	j = 0;
	res = true;
	while (cmd->args[i])
	{
		if(!check_key_export(cmd->args[i]))
		{
			// shell->exit_status = 1;
			res = false;
		}
		else
		{
			j = check_key_export(cmd->args[i]);
			arg_to_env(shell, cmd->args[i], j);
		}
		i++;
	}
	return (res);
}


int	ft_export(t_shell *shell, t_cmd *cmd, int fd)
{
	(void) fd;

	if (!cmd->args[1])
		print_env_export(shell->env);
	if (!export_args(shell, cmd))
		return (1);
	return (0);
}
