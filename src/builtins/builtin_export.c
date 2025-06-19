/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 15:15:31 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/19 16:50:55 by tjacquel         ###   ########.fr       */
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

int check_arg_export(t_shell *shell, t_cmd *cmd)
{
	int	i;
	int j;

	i = 1;
	j = 0;
	while(cmd->args[i])
	{
		while(cmd->args[i][j] && cmd->args[i][j] != '=')
		{
			if(!is_valid_keychar(cmd->args[i][j]) || ft_isdigit(cmd->args[i][0]))
			{
				printf("export: `%s': not a valid identifier\n", cmd->args[i]);
				break ;
			}
			j++;
		}
		i++;
	}
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
	if(check_arg_export(shell, cmd))
			return (shell->exit_status =1, false);
	// split jusqauu  '='
	// ajouter le noeud
	// noeud add back
	return (true);
}

// Implémenter la commande builtin `export`
// Ajouter ou modifier une variable d’environnement
