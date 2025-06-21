/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:42:46 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/21 17:03:01 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// COPIE DES FONCTIONS LIB AVEC GC
char	*gc_strndup(char *src, size_t n, t_gc_type type)
{
	char	*dest;
	size_t		i;

	dest = gc_mem(GC_ALLOC, n + 1, NULL, type);
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

char	*gc_strdup(const char *src, t_gc_type type)
{
	char	*dup;
	size_t	len;

	if (!src)
		return (NULL);
	len = ft_strlen(src) + 1;
	dup = gc_mem(GC_ALLOC, len, NULL, type);
	if (!dup)
		error_exit("gc_strdup: alloc failed");
	ft_strlcpy(dup, src, len);
	return (dup);
}

char	**gc_split(char *str, char sep, t_gc_type type)
{
	char	**tmp;
	char	**gc_tab;
	int		i;

	tmp = ft_split(str, sep);
	if (!tmp)
		error_exit("gc_split: split failed");
	i = 0;
	while (tmp[i])
		i++;
	gc_tab = gc_mem(GC_ALLOC, sizeof(char *) * (i + 1), NULL, type);
	if (!gc_tab)
		error_exit("gc_split: alloc tableau gc_tab failed");
	i = 0;
	while (tmp[i])
	{
		gc_tab[i] = gc_strdup(tmp[i], type);
		free(tmp[i]);
		i++;
	}
	gc_tab[i] = NULL;
	free(tmp);
	return (gc_tab);
}

char	*gc_strjoin(char const *s1, char const *s2, t_gc_type type)
{
	char	*joined;
	size_t	len1;
	size_t	len2;
	size_t	i;
	size_t	j;

	if (!s1 && !s2)
		return (NULL);
	len1 = 0;
	len2 = 0;
	if (s1)
		len1 = ft_strlen(s1);
	if (s2)
		len2 = ft_strlen(s2);
	joined = gc_mem(GC_ALLOC, sizeof(char) * (len1 + len2 + 1), NULL, type);
	if (!joined)
		return (NULL);
	i = 0;
	j = 0;
	while (s1 && s1[i])
	{
		joined[i] = s1[i];
		i++;
	}
	while (s2 && s2[j])
	{
		joined[i + j] = s2[j];
		j++;
	}
	joined[i + j] = '\0';
	return (joined);
}

static size_t	count_size(long nb)
{
	size_t	size;

	size = 0;
	if (nb < 0)
	{
	nb = nb * (-1);
		size = 1;
	}
	if (nb == 0)
		size = 1;
	else
	{
		while (nb)
		{
			nb = nb / 10;
			size++;
		}
	}
	return (size);
}

char	*gc_itoa(int n, t_gc_type type)
{
	size_t	size;
	long	nb;
	char	*str;
	int		is_negative;

	size = count_size((long) n);
	str = gc_mem(GC_ALLOC, (size + 1), NULL, type);
	// str = (char *) malloc(sizeof(char) * (size + 1));
	if (str == NULL)
		return (NULL);
	nb = (long) n;
	is_negative = 0;
	if (nb < 0)
	{
		nb = nb * (-1);
		str[0] = '-';
		is_negative = 1;
	}
	str[size] = '\0';
	while (size > (size_t) is_negative)
	{
		str[size - 1] = nb % 10 + '0';
		nb = nb / 10;
		size--;
	}
	return (str);
}
