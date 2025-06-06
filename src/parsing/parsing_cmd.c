/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 14:24:49 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/06 15:41:38 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	add_arg(t_token *temp)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
}

int	parse_tokens(t_token *token_list)
{
	t_token *temp;

	if (!token_list) // si ya pas de token_list nsm on se casse
		return (false);
	temp = token_list;
	while (temp->prev) // on remonte la liste ofc!!
		temp = temp->prev;

	while (temp->next) // on parcourt la chaine jusquau dernier noeud
	{
		add_arg(temp);
		if (temp->token_type == TOKEN_PIPE && temp->next)  // si on tombe sur un PIPE on passe a la commande suivante. bien sur faut que la prochain noeud existe
			temp = temp->next;
		temp = temp->next;
	}
	return (true);
}
