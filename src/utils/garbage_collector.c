/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 15:27:22 by aumartin          #+#    #+#             */
/*   Updated: 2025/05/06 09:41:40 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Ajoute un noeud en tete de liste GC
static void	gc_add_node(t_gc *gc, void *ptr, t_gc_type type)
{
	t_gc_node	*new;

	new = ft_calloc(1, sizeof(t_gc_node));
	if (!new)
		return ;
	new->ptr = ptr;
	new->type = type;
	new->next = gc->head;
	gc->head = new;
}

// Alloue avec ft_calloc et enregistre
static void	*gc_alloc(t_gc *gc, size_t size, t_gc_type type)
{
	void	*ptr;

	ptr = ft_calloc(1, size);
	if (!ptr)
		return (NULL);
	gc_add_node(gc, ptr, type);
	return (ptr);
}

// Libère un pointeur donné
static void	gc_free_one(t_gc *gc, void *ptr)
{
	t_gc_node	*curr;
	t_gc_node	*prev;

	curr = gc->head;
	prev = NULL;
	while (curr)
	{
		if (curr->ptr == ptr)
		{
			if (prev)
				prev->next = curr->next;
			else
				gc->head = curr->next;
			free(curr->ptr);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

// Libère tout ou par type
static void	gc_free_all(t_gc *gc, t_gc_type type)
{
	t_gc_node	*curr;
	t_gc_node	*tmp;
	t_gc_node	*prev;

	curr = gc->head;
	prev = NULL;
	while (curr)
	{
		if (type == GC_NONE || curr->type == type)
		{
			tmp = curr;
			if (prev)
				prev->next = curr->next;
			else
				gc->head = curr->next;
			curr = curr->next;
			free(tmp->ptr);
			free(tmp);
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
}

// Gestion memoire
void	*gc_mem(t_gc_action op, size_t size, void *ptr, t_gc_type type)
{
	t_gc	*gc;

	gc = &get_shell()->gc;
	if (op == GC_ALLOC)
		return (gc_malloc(gc, size, type));
	else if (op == GC_FREE_ONE)
		return (gc_free_one(gc, ptr), NULL);
	else if (op == GC_FREE_ALL)
		return (gc_free_all(gc, type), NULL);
	return (NULL);
}

/* Exemple d’utilisation dans le code :
char *s1 = gc_mem(GC_ALLOC, 100, NULL, GC_TMP);		// alloue 100 octets
gc_mem(GC_FREE_ONE, 0, s1, 0);						// libère s1
gc_mem(GC_FREE_ALL, 0, NULL, GC_CMD);				// libère tous les GC_CMD
gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);				// libère tout
 */
