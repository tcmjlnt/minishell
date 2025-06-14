/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 20:37:55 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/14 20:43:37 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_xpnd	*ft_lstlast_xpnd(t_xpnd *xpnd)
{
	while (xpnd && xpnd->next)
		xpnd = xpnd->next;
	return (xpnd);
}

void	ft_lstadd_back_xpnd(t_xpnd **xpnd, t_xpnd *new)
{
	t_xpnd	*temp;

	if (xpnd == NULL || new == NULL)
		return ;
	if (*xpnd == NULL)
	{
		*xpnd = new;
		new->prev = NULL;
	}
	else
	{
		temp = ft_lstlast_xpnd(*xpnd);
		new->prev = temp;
		temp->next = new;
	}
}

t_xpnd	*ft_lstnewxpnd(void)
{
	t_xpnd	*new_xpnd;

	new_xpnd = malloc(sizeof(t_xpnd));
	if (!new_xpnd)
		return (NULL);
	new_xpnd->substr = NULL;
	new_xpnd->xpnd_check = false;
	new_xpnd->str_to_join = NULL;
	new_xpnd->prev = NULL;
	new_xpnd->next = NULL;

	return (new_xpnd);
}