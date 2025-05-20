/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 16:25:25 by aumartin          #+#    #+#             */
/*   Updated: 2025/05/20 18:11:18 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_shell	*get_shell(void)
{
	static t_shell	shell;

	return (&shell);
}

void	init_shell(void)
{
	t_shell	*shell;

	shell = get_shell();
	ft_bzero(shell, sizeof(t_shell));
}

// t_token	*init_token(t_token	*token)
// {
// 	token = malloc(sizeof(t_token));
// 	if (!token)
// 		return ;
// 	token->type = -1;
// 	token->node_num = 0;
// 	token->value = NULL;
// 	token->prev = NULL;
// 	token->next = NULL;
// 	return (token);
// }
