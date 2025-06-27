/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 17:23:52 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/27 18:57:59 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	free_env_node(t_env *node)
{
	if (!node)
		return ;
	free(node->key);
	free(node->value);
	free(node);
}

static void	del_env_node(t_env **env_list, const char *key)
{
	t_env	*curr_node;
	t_env	*prev_node;

	if (!env_list || !(*env_list) || !key)
		return ;
	curr_node = *env_list;
	if (ft_strcmp(curr_node->key, key) == 0)
	{
		*env_list = curr_node->next;
		return (free_env_node(curr_node));
	}
	prev_node = curr_node;
	curr_node = curr_node->next;
	while (curr_node)
	{
		if (ft_strcmp(curr_node->key, key) == 0)
		{
			prev_node->next = curr_node->next;
			return (free_env_node(curr_node));
		}
		prev_node = curr_node;
		curr_node = curr_node->next;
	}
}

int	ft_unset(t_shell *shell, t_cmd *cmd, int fd)
{
	size_t	i;

	(void) fd;
	i = 1;
	if (cmd->args[1])
	{
		while (cmd->args[i])
		{
			del_env_node(&shell->env, cmd->args[i]);
			i++;
		}
	}
	shell->exit_status = 0;
	return (0);
}
