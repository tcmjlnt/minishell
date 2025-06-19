/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:42:46 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/18 22:53:24 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// COPIE DES FONCTIONS LIB AVEC GC
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
