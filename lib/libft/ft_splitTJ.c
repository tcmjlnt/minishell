/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_splitTJ.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 19:22:32 by tjacquel          #+#    #+#             */
/*   Updated: 2025/05/29 19:22:09 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i] == c && s[i])
		i++;
	while (s[i])
	{
		if (s[i] != c)
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
		else
		{
			while (s[i] == c)
				i++;
		}
	}
	return (count);
}

static void	free_all(char **dest, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		free(dest[i]);
		i++;
	}
	free(dest);
}

static size_t	word_len(char const *s, char c)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

char	**ft_splitTJ(char const *s, char c)
{
	char	**dest;
	size_t	i;
	size_t	word_index;
	size_t	words_count;

	if (s == NULL)
		return (NULL);
	words_count = count_words(s, c);
	dest = ft_calloc((words_count + 1), sizeof(char *));
	if (!dest)
		return (NULL);
	i = 0;
	word_index = 0;
	while (word_index < words_count)
	{
		while (s[i] == c)
			i++;
		dest[word_index] = ft_substr(s, i, word_len(&s[i], c));
		if (dest[word_index] == NULL)
			return (free_all(dest, words_count + 1), NULL);
		i = i + word_len(&s[i], c);
		word_index++;
	}
	dest[word_index] = NULL;
	return (dest);
}
