/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 19:01:14 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/02 18:27:37 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_token(t_token	*token)
{
	t_token	*temp;


	if (!token)
		return ;
	temp = token;
	while(temp->prev)
	{
		temp = temp->prev;
	}
	int i = 0;
	while (temp)
	{
		printf("arg[%d]: %s ; token_type: %d\n", i, temp->token_value, temp->token_type);
		i++;
		temp = temp->next;
	}
}

t_token	*ft_lstnewtoken(char *prompt, int n, t_token_type token_type)
{
	t_token *new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->token_type = token_type;
	new_token->token_value = ft_strndup(prompt, n);
	// if (new_token->prev && new_token->node_num != 0)
	// 	new_token->node_num = new_token->prev->node_num++; // ca segfault
	new_token->prev = NULL;
	new_token->next = NULL;

	return (new_token);

}

t_token	*ft_lstlast_token(t_token *token)
{
	while (token && token->next)
		token = token->next;
	return (token);
}


// void	ft_lstadd_back_token(t_token **token, t_token *new)
// {
// 	if (!token || !new)
// 		return ;
// 	if (*token)
// 		ft_lstlast_token(*token)->next = new;
// 	else
// 		*token = new;
// }

void	ft_lstadd_back_token(t_token **token, t_token *new)
{
	t_token	*temp;

	if (token == NULL || new == NULL)
		return ;
	if (*token == NULL)
		*token = new;
	else
	{
		temp = ft_lstlast_token(*token);
		new->prev = temp;
		temp->next = new;
	}
}