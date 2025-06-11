/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:53:36 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/11 18:54:02 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_redir	*ft_lstnewredir(void)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return (NULL);
	new_redir->file = NULL;
	new_redir->type = -666; // check ce qu'on met
	new_redir->next = NULL;
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
	if (!redir || !new)
		return ;
	if (*redir)
		ft_lstlast_redir(*redir)->next = new;
	else
		*redir = new;
}

int	fill_redir(t_redir **redir_list, t_cmd *cmd, t_token *token)
{
	t_redir	*current_redir;

	(void)token;
	(void)cmd;
	current_redir = ft_lstnewredir();
	if (!current_redir)
		return (false);
	current_redir->file = gc_strdup(token->token_value, GC_CMD);
	current_redir->type = token->prev->token_type;
	ft_lstadd_back_redir(redir_list, current_redir);
	//printf("file redir : %s\n", current_redir->file);
	return (true);
}


