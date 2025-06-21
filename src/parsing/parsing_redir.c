/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:53:36 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/21 16:43:47 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_redir	*ft_lstnewredir(void)
{
	t_redir	*new_redir;

	new_redir = gc_mem(GC_ALLOC, sizeof(t_redir), NULL, GC_CMD);
	// new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (NULL);
	new_redir->file = NULL;
	new_redir->delim = NULL;
	new_redir->type = -666; // check ce qu'on met
	new_redir->next = NULL;
	new_redir->prev = NULL;

	return (new_redir);
}

t_redir	*ft_lstlast_redir(t_redir *redir)
{
	while (redir && redir->next)
		redir = redir->next;
	return (redir);
}

void	ft_lstadd_back_redir(t_redir **redir, t_redir *new)
{
	t_redir	*temp;

	if (redir == NULL || new == NULL)
		return ;
	if (*redir == NULL)
	{
		*redir = new;
		new->prev = NULL;
	}
	else
	{
		temp = ft_lstlast_redir(*redir);
		new->prev = temp;
		temp->next = new;
	}
}

int	fill_redir(t_redir **redir_list, t_token *token)
{
	t_redir	*current_redir;

	current_redir = ft_lstnewredir();
	if (!current_redir)
		return (false);
	if (token->prev && token->prev->token_type != TOKEN_REDIRECT_HEREDOC)
	{
		current_redir->file = gc_strdup(token->token_value, GC_CMD);
		if (!current_redir->file)
		{
			free(current_redir);
			return (false);
		}
	}
	else if (token->prev && token->prev->token_type == TOKEN_REDIRECT_HEREDOC)
	{
			current_redir->delim = gc_strdup(token->token_value, GC_CMD);
			if (!current_redir->delim)
			{
				free(current_redir);
				return (false);
			}
	}
	current_redir->type = token->prev->token_type;
	ft_lstadd_back_redir(redir_list, current_redir);
	return (true);
}


