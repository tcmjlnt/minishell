/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:42:46 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/27 19:08:00 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// COPIE DES FONCTIONS LIB AVEC GC
char	*gc_strndup(char *src, size_t n, t_gc_type type)
{
	char	*dest;
	size_t	i;

	dest = gc_mem(GC_ALLOC, n + 1, NULL, type);
	if (!dest)
		error_free_gc("gc_strndup malloc failure\n");
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
		error_free_gc("gc_strdup: alloc failed");
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
		error_free_gc("gc_split: split failed");
	i = 0;
	while (tmp[i])
		i++;
	gc_tab = gc_mem(GC_ALLOC, sizeof(char *) * (i + 1), NULL, type);
	if (!gc_tab)
		error_free_gc("gc_split: alloc tableau gc_tab failed");
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
	char	*join;
	size_t	len_s1;
	size_t	len_s2;

	if (!s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	join = gc_mem(GC_ALLOC, (len_s1 + len_s2 + 1) * sizeof(char), NULL, type);
	if (!join)
		return (error_exit("gc_strjoin: allocation failed"), NULL);
	ft_strlcpy(join, s1, (len_s1 + 1));
	ft_strlcat(join, s2, len_s1 + len_s2 + 1);
	return (join);
}
