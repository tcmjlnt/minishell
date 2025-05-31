/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:24:30 by tjacquel          #+#    #+#             */
/*   Updated: 2025/05/31 19:14:32 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../..//include/minishell.h"


char	*ft_strndup(char *src, size_t n)
{
	char	*dest;
	size_t		i;

	dest = malloc(sizeof(char) * (n + 1));
	if (!dest)
		return(NULL);
	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

t_token	*ft_lstnew_token(char *value, int type, int node_num)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->token_type = type;
	new->token_value = value;
	new->node_num = node_num;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	print_token(t_token	*token)
{
	t_token	*temp;


	temp = token;
	while(temp->prev)
	{
		temp = temp->prev;
	}
	while (temp)
	{
		printf("%d\n%s\n%d\n", temp->token_type, temp->token_value, temp->node_num);
		temp = temp->next;
	}
}