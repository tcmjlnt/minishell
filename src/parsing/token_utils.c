/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 17:43:55 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/02 19:47:14 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

const char	*token_type_string(t_token_type token_type)
{
	if (token_type == TOKEN_PIPE)
		return ("|");
	else if (token_type == TOKEN_REDIRECT_IN)
		return ("<");
	else if (token_type == TOKEN_REDIRECT_OUT)
		return (">");
	else if (token_type == TOKEN_REDIRECT_APPEND)
		return (">>");
	else if (token_type == TOKEN_REDIRECT_HEREDOC)
		return ("<<");
	else
		return("WTF IS GOING ON");
}

int	is_operator_token(t_token *token)
{
	if (!token)
		return (false);
	if (token->token_type == TOKEN_PIPE ||
		 token->token_type == TOKEN_REDIRECT_IN ||
		 token->token_type == TOKEN_REDIRECT_OUT ||
		 token->token_type == TOKEN_REDIRECT_APPEND ||
		 token->token_type == TOKEN_REDIRECT_HEREDOC)
		 return (true);
	else
		return (false);
}

int	check_token(t_token *token)
{
	t_token	*temp;

	if (!token)
		return (false);
	temp = token;
	while (temp->prev)
		temp = temp->prev;
	while (temp)
	{
		if (is_operator_token(temp) && temp->next && is_operator_token(temp->next))
		{
			printf("minishell: syntax error near unexpected token `%s'\n", token_type_string(temp->next->token_type));
			return (false);
		}
		temp = temp->next;
	}
	return (true);
}