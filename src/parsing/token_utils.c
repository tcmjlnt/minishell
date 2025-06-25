/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 17:43:55 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/25 22:12:25 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// int	is_redir_operator(int token_type) // helper function creee ailleurs
// {
// 	if (token_type == TKN_APPEND || token_type == TKN_HEREDOC
// 		|| token_type == TKN_IN || token_type == TKN_OUT)
// 		return (true);
// 	else
// 		return (false);
// }

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
	if (token_type == TKN_PIPE)
		return ("|");
	else if (token_type == TKN_IN)
		return ("<");
	else if (token_type == TKN_OUT)
		return (">");
	else if (token_type == TKN_APPEND)
		return (">>");
	else if (token_type == TKN_HEREDOC)
		return ("<<");
	else
		return("WTF IS GOING ON");
}

int	is_operator_token(t_token *token)
{
	if (!token)
		return (false);
	if (token->token_type == TKN_PIPE ||
		 token->token_type == TKN_IN ||
		 token->token_type == TKN_OUT ||
		 token->token_type == TKN_APPEND ||
		 token->token_type == TKN_HEREDOC)
		 return (true);
	else
		return (false);
}

int	is_redir_operator(t_token_type token_type)
{
	if (token_type == TKN_IN || token_type == TKN_OUT
		|| token_type == TKN_APPEND
		|| token_type == TKN_HEREDOC)
		return (true);
	return (false);
}

int	check_token(t_token **token_list)
{
	t_token	*temp;

	if (!token_list || !(*token_list))
		return (false);
	temp = *token_list;
	while (temp->prev)
		temp = temp->prev;
	while (temp)
	{
		if ((is_redir_operator(temp->token_type) && temp->next && is_redir_operator(temp->next->token_type))
			|| (is_operator_token(temp) && temp->next && temp->next->token_type == TKN_PIPE))
		{
			printf("minishell: syntax error near unexpected token `%s'\n", token_type_string(temp->next->token_type));
			return (false);
		}
		else if (is_operator_token(temp) && temp->next && is_operator_token(temp->next)
				&& temp->next->next && is_operator_token(temp->next->next))
		{
			printf("minishell: syntax error near unexpected token `%s'\n", token_type_string(temp->next->token_type));
			return (false);
		}
		else if (is_operator_token(temp) && !temp->next)
		{
			printf("minishell: syntax error near unexpected token `newline'\n");
			return (false);
		}
		temp = temp->next;
	}
	return (true);
}


