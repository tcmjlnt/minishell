/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 19:01:14 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/12 21:44:58 by tjacquel         ###   ########.fr       */
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
		printf("token[%d]: `%s` ; token_raw: `%s` ; token_type: %d\n", i, temp->token_value, temp->token_raw, temp->token_type);
		// printf("token[%d]: `%s`\n", i, temp->token_value);

		i++;
		temp = temp->next;
	}
}

t_cmd	*ft_lstnewcmd(void)
{
	t_cmd	*new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->args = malloc(sizeof(char *) * (MAX_ARGS + 1));
	if (!new_cmd->args)
	{
		// free etc.
		// gc_free_all
		// exit
		return (NULL);
	}
	new_cmd->cmd = NULL;
	new_cmd->args[0] = NULL;
	new_cmd->fd_in = STDIN_FILENO;
	new_cmd->fd_out = STDOUT_FILENO;
	new_cmd->pid = 0;
	new_cmd->is_builtin = false;
	new_cmd->redir = NULL;

	// if (token_type == TOKEN_PIPE || token_type == TOKEN_WORD)
	// {
	// 	new_cmd->args[(*j)] = ft_strdup(token->token_value);
	// }
	// if (token_type == TOKEN_PIPE)
	// {
	// 	new_cmd->cmd = ft_strdup(token->token_value);
	// }
	new_cmd->prev = NULL;
	new_cmd->next = NULL;

	return (new_cmd);

}

t_token	*ft_lstnewtoken(char *prompt, int n, t_token_type token_type)
{
	t_token *new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->token_type = token_type;
	if (token_type != TOKEN_WORD)
		new_token->token_value = ft_strndup(prompt, n);
	else
		new_token->token_value = ft_strndup_noquotes(prompt, n);
	new_token->token_raw = ft_strndup(prompt, n);

	// if (new_token->prev && new_token->node_num != 0)
	// 	new_token->node_num = new_token->prev->node_num++; // ca segfault
	new_token->prev = NULL;
	new_token->next = NULL;

	return (new_token);

}

t_cmd	*ft_lstlast_cmd(t_cmd *cmd)
{
	while (cmd && cmd->next)
		cmd = cmd->next;
	return (cmd);
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

void	ft_lstadd_back_cmd(t_cmd **cmd, t_cmd *new)
{
	t_cmd	*temp;

	if (cmd == NULL || new == NULL)
		return ;
	if (*cmd == NULL)
	{
		*cmd = new;
		new->prev = NULL;
	}
	else
	{
		temp = ft_lstlast_cmd(*cmd);
		new->prev = temp;
		temp->next = new;
	}
}

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