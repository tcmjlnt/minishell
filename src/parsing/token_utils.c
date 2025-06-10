/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 17:43:55 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/10 18:30:04 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_redir_operator(int token_type)
{
	if (token_type == TOKEN_REDIRECT_APPEND || token_type == TOKEN_REDIRECT_HEREDOC
		|| token_type == TOKEN_REDIRECT_IN || token_type == TOKEN_REDIRECT_OUT)
		return (true);
	else
		return (false);
}

int	is_inside_dquotes(char *token_raw)
{
	int i = 0;
	int in_single = 0;
	int in_double = 0;

	while (token_raw[i])
	{
		if (token_raw[i] == '\'' && !in_double)
			in_single = !in_single; // toggle in_single state
		else if (token_raw[i] == '\"' && !in_single)
			in_double = !in_double; // toggle in_double state
		i++;
	}
	return (in_double);
}

int	is_inside_squotes(char *token_raw)
{
	int i = 0;
	int in_single = 0;
	int in_double = 0;

	while (token_raw[i])
	{
		if (token_raw[i] == '\'' && !in_double)
			in_single = !in_single; // toggle in_single state
		else if (token_raw[i] == '\"' && !in_single)
			in_double = !in_double; // toggle in_double state
		i++;
	}
	return (in_single);
}

const char	*token_type_string(t_token_type token_type) // elle est pas utilisee cette fonction??
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