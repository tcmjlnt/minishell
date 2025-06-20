/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:15:31 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/20 14:29:25 by tjacquel         ###   ########.fr       */
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
		// else
		// 	printf("\"\"");
		printf("\n");
		env = env->next;
	}
}



size_t check_key_export(t_shell *shell, char *arg)
{
	size_t	j;

	j = 0;

	while(arg[j] && arg[j + 1] && arg[j + 1] != '=')
	{
		if(!is_valid_keychar(arg[j]) || ft_isdigit(arg[0]) || arg[0] == '=')
		{
			shell->exit_status = 1;
			write(2, "minishell: export: `", 20);
			write(2, arg, ft_strlen(arg));
			write(2, "': not a valid identifier\n", 26);
			return (0);
		}
		j++;
	}
	return (++j);
}
int	arg_to_env(t_shell *shell, char *arg, size_t n)
{
	(void) shell;
	char	*key;
	char	*value;
	t_env	*env_new_node;

	key = NULL;
	value = NULL;
	key = ft_strndup(arg, n);
	value = ft_strdup(arg + n + 1);

	printf("key: `%s`, value: `%s`\n", key, value);
	env_new_node = env_new(key, value, true);
	env_add_back((t_env **)&shell->env, env_new_node);

	return (1);

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
		if(!check_key_export(shell, cmd->args[i]))
		{
			shell->exit_status = 1;
			res = false;
		}
		else
		{
			j = check_key_export(shell, cmd->args[i]);
			printf ("arg[%td]: key_strlen=%td\n", i, j);
			arg_to_env(shell, cmd->args[i], j);
		}
		i++;
	}
	return (res);
}


int	ft_export(t_shell *shell, t_cmd *cmd, int fd)
{
	//t_env	*new_env;

	(void) cmd;
	(void) fd;
	// printf("export ");

	// new_env = env_new(cmd->args[1])
	if (!cmd->args[1])
		print_env_export(shell->env);
	if (!export_args(shell, cmd))
		return (false);
	// split jusqauu  '='
	// ajouter le noeud
	// noeud add back
	return (true);
}

// Implémenter la commande builtin `export`
// Ajouter ou modifier une variable d’environnement
