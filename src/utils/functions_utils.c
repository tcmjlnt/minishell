/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:24:30 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/06 20:07:17 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../..//include/minishell.h"

size_t	ft_strlen_noquotes(char *src, size_t n)
{
	size_t	i;
	size_t	j;
	int	in_single = 0;
	int	in_double = 0;

	i = 0;
	j = 0;
	while (src[i] && i < n)
	{

		if (is_quote(src[i]))
		{
			if (src[i] == '\'' && !in_double) // je recontre une S_QUOTE et je NE suis PAS dans une D_QUOTE
				in_single = !in_single; // toggle in_single state
			else if (src[i] == '\"' && !in_single) // je rencontre une D_QUOTE et je NE suis PAS dans une S_QUOTE
				in_double = !in_double; // toggle in_double state
			else if (src[i] == '\"' && in_single) // je rencontre une D_QUOTE et je suis dans une S_QUOTE
				j++;
			else if (src[i] == '\'' && in_double) // je rencontre une S_QUOTE et je suis dans une D_QUOTE
				j++;
			i++;
		}
		else
		{
			j++;
			i++;
		}
	}
	return (j);
}

char	*ft_strndup_noquotes(char *src, size_t n)
{
	char		*dest;
	size_t		i;
	size_t		j;
	size_t		len_noquotes;
	int	in_single = 0;
	int	in_double = 0;

	len_noquotes = ft_strlen_noquotes(src, n);
	dest = malloc(sizeof(char) * (len_noquotes + 1));
	if (!dest)
		return(NULL);
	i = 0;
	j = 0;
	while (i < n && src[i])
	{
		if (is_quote(src[i]))
		{
			if (src[i] == '\'' && !in_double) // je recontre une S_QUOTE et je NE suis PAS dans une D_QUOTE
				in_single = !in_single; // toggle in_single state
			else if (src[i] == '\"' && !in_single) // je rencontre une D_QUOTE et je NE suis PAS dans une S_QUOTE
				in_double = !in_double; // toggle in_double state
			else if (src[i] == '\"' && in_single) // je rencontre une D_QUOTE et je suis dans une S_QUOTE
				dest[j++] = src[i]; // pour gagner 3 ligne de code { j++}
			else if (src[i] == '\'' && in_double) // je rencontre une S_QUOTE et je suis dans une D_QUOTE
				dest[j++] = src[i];
			i++;
		}
		else
		{
			if (j < len_noquotes)
			{
				dest[j] = src[i];
				j++;
			}
			i++;
		}
	}
	dest[j] = '\0';
	return (dest);
}

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

/* t_token	*ft_lstnew_token(char *value, int type) //, int node_num)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->token_type = type;
	new->token_value = value;
	// new->node_num = node_num;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}
 */