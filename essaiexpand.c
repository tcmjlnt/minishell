/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   essaiexpand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 18:27:22 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/15 21:59:49 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
// #include <stdbool.h>
// #include "include/minishell.h"

typedef enum e_bool
{
	false,
	true
}	t_bool;

typedef	struct s_xpnd
{
	char			*substr;
	// char			*key;
	t_bool			xpnd_check;
	// char			*xpnd_value;
	char			*str_to_join;
	// t_token			*token_link; // jai peur que ce soit complique a gerer notamment dans les cas ou je ne veux pas expand
	struct s_xpnd	*prev;
	struct s_xpnd	*next;
}	t_xpnd;

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_inside_squotes(char *token_raw, size_t pos)
{
	size_t i = 0;
	int in_single = 0;
	int in_double = 0;

	while (i < pos && token_raw[i])
	{
		if (token_raw[i] == '\'' && !in_double)
			in_single = !in_single; // toggle in_single state
		else if (token_raw[i] == '\"' && !in_single)
			in_double = !in_double; // toggle in_double state
		i++;
	}
	return (in_single);
}

// static int	is_inside_quotes(char *prompt, size_t pos)
// {
// 	size_t	i = 0;
// 	int	in_single = 0;
// 	int	in_double = 0;

// 	while (i < pos && prompt[i])
// 	{
// 		if (prompt[i] == '\'' && !in_double)
// 			in_single = !in_single; // toggle in_single state
// 		else if (prompt[i] == '\"' && !in_single)
// 			in_double = !in_double; // toggle in_double state
// 		i++;
// 	}
// 	return (in_single || in_double);
// }

size_t	ft_strnlen_noquotes(char *src, size_t n)
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

	len_noquotes = ft_strnlen_noquotes(src, n);
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

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

static int	ft_isalnum(int c)
{
	if (ft_isalpha(c) || ft_isdigit(c))
		return (1);
	else
		return (0);
}

static int is_valid_keychar(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	else
		return (0);
}

char	**char_keys(char *tkn_raw, size_t count_expand)
{
	size_t	i;
	size_t	start;
	size_t	count_keys;

	t_bool	in_single;
	t_bool	in_double;
	char	**key = malloc(sizeof(char *) * (count_expand + 1));
	if (!key)
		return (NULL);
	i = 0;
	start = 0;
	count_keys = 0;

	in_double = false;
	in_single = false;
	while(tkn_raw[i])
	{
		if (tkn_raw[i] == '\'' && !in_double)
			in_single = !in_single; // toggle in_single state
		else if (tkn_raw[i] == '\"' && !in_single)
			in_double = !in_double; // toggle in_double state
		if (tkn_raw[i] == '$' && tkn_raw[i + 1] && is_valid_keychar(tkn_raw[i + 1])
				&& !in_single)
		{
			i++;
			start = i;
			while (tkn_raw[i] && is_valid_keychar(tkn_raw[i]))
				i++;
			key[count_keys] = ft_strndup(tkn_raw + start, i - start);
			if (!key[count_keys])
			{
				while (count_keys > 0)
					free(key[--count_keys]);
				free(key);
				return (NULL);
			}
			count_keys++;
			if (!tkn_raw[i])
				break ;
		}
		else
			i++;
	}
	key[count_expand] = NULL;
	return (key);
}

// int	inside_potential_xpnd_key(char *str, size_t i, size_t end)
// {

// }
// int	tkn_xpnd_segmentation_test(char *tkn_raw, t_xpnd **xpnd_list)
// {
// 	size_t	i;
// 	size_t	start;
// 	t_xpnd	*current_xpnd;


// 	i = 0;
// 	start = 0;


// 	while(tkn_raw[i])
// 	{

// 		start = i;

// 		while (tkn_raw[i] != '$' && !is_inside_squotes(tkn_raw, i))
// 		{
// 			if (tkn_raw[i] == '$' && tkn_raw[i + 1] && !is_valid_keychar(tkn_raw[i + 1]) && !is_inside_squotes(tkn_raw, i))
// 				break ;
// 			while (tkn_raw[i] && is_valid_keychar(tkn_raw[i]))
// 			{
// 				printf("tkn_raw[i]: %c	;	i: %zd\n", tkn_raw[i], i);
// 				i++;
// 			}			// printf("tkn_raw[i]: %c	;	i: %zd	;	in_single: %d	;	in_double: %d\n", tkn_raw[i], i, in_single, in_double);
// 			// i++;
// 		}
// 		else
// 			i++;
// 	}
// 	current_xpnd = ft_lstnewxpnd();
// 	if (!current_xpnd)
// 		return (false);
// 	current_xpnd->substr = ft_strndup(tkn_raw + start, i - start);
// 	current_xpnd->xpnd_check = false;

// 	ft_lstadd_back_xpnd(xpnd_list, current_xpnd);
// 	return (true);
// }

int	tkn_xpnd_segmentation(char *tkn_raw, t_xpnd **xpnd_list)
{
	size_t	i;
	size_t	start;
	// size_t	count_keys;
	t_xpnd	*current_xpnd;

	// t_bool	in_single;
	// t_bool	in_double;

	i = 0;
	start = 0;
	// count_keys = 0;

	// in_double = false;
	// in_single = false;
	while(tkn_raw[i])
	{
		// if (tkn_raw[i] == '\'' && !in_double)
		// 	in_single = !in_single; // toggle in_single state
		// else if (tkn_raw[i] == '\"' && !in_single)
		// 	in_double = !in_double; // toggle in_double state

		if (tkn_raw[i] == '$' && tkn_raw[i + 1] && is_valid_keychar(tkn_raw[i + 1])
				&& !is_inside_squotes(tkn_raw, i))
		{
			if (i > start)
			{
				current_xpnd = ft_lstnewxpnd();
				if (!current_xpnd)
					return (false);
				current_xpnd->substr = ft_strndup(tkn_raw + start, i - start);
				current_xpnd->xpnd_check = false;
				ft_lstadd_back_xpnd(xpnd_list, current_xpnd);
			}
			{
				i++;
				start = i;
				while (tkn_raw[i] && is_valid_keychar(tkn_raw[i]))
				{
					printf("tkn_raw[i]: %c	;	i: %zd\n", tkn_raw[i], i);
					i++;
				}
				current_xpnd = ft_lstnewxpnd();
				if (!current_xpnd)
					return (false);
				current_xpnd->substr = ft_strndup(tkn_raw + start, i - start);
				current_xpnd->xpnd_check = true;
				// je me suis arrete la --> il faut 1. remplir la liste avec des noeuds les substrs qui ne sont pas xpnd_check =true (else)
				// 1b ajouter le noeud a la liste
				// il faut 2. recuperer la valeur de la cle depuis l'env. ca peut etre fait dans cette boucle ou en dehors pour plus concis
				// count_keys++;
				ft_lstadd_back_xpnd(xpnd_list, current_xpnd);
				start = i;
			}
		}
		else
			i++;
	}
	if (i > start) {
		current_xpnd = ft_lstnewxpnd();
		if (!current_xpnd)
			return (false);
		current_xpnd->substr = ft_strndup(tkn_raw + start, i - start);
		current_xpnd->xpnd_check = false;
		ft_lstadd_back_xpnd(xpnd_list, current_xpnd);
	}
	return (true);
}

size_t	count_expand(char *tkn_raw)
{
	size_t	i;
	size_t	count_expand;
	t_bool	in_single;
	t_bool	in_double;

	i = 0;
	count_expand = 0;
	in_double = false;
	in_single = false;
	while(tkn_raw[i])
	{
		if (tkn_raw[i] == '\'' && !in_double)
			in_single = !in_single; // toggle in_single state
		else if (tkn_raw[i] == '\"' && !in_single)
			in_double = !in_double; // toggle in_double state
		if (tkn_raw[i] == '$' && tkn_raw[i + 1] && is_valid_keychar(tkn_raw[i + 1])
				&& !in_single)
			count_expand++;
		else
		{
			// while ()
			//count_expand++;
		}
		i++;
	}
	return (count_expand);
}


size_t	count_segment(char *tkn_raw)
{
	size_t	i;
	size_t	count_segment;
	size_t	current_literal_length;
	t_bool	in_single;
	t_bool	in_double;

	i = 0;
	count_segment = 0;
	current_literal_length = 0;
	in_double = false;
	in_single = false;
	while(tkn_raw[i])
	{
		if (tkn_raw[i] == '\'' && !in_double)
			in_single = !in_single; // toggle in_single state
		else if (tkn_raw[i] == '\"' && !in_single)
			in_double = !in_double; // toggle in_double state
		if (tkn_raw[i] == '$' && tkn_raw[i + 1] && is_valid_keychar(tkn_raw[i + 1])
				&& !in_single)
		{
			if (current_literal_length > 0)
				count_segment++;
			current_literal_length = 0;
			count_segment++;
			i++;
			while (tkn_raw[i] && is_valid_keychar(tkn_raw[i]))
				i++;
			continue ;
		}
		else
		{
			current_literal_length++;
			i++;
		}
	}
	if (current_literal_length > 0)
		count_segment++;
	return (count_segment);
}

void	printf_xpnd(t_xpnd **xpnd_list)
{
	t_xpnd *xpnd_current;
	int i = 0;

	xpnd_current = *xpnd_list;
	if (!xpnd_current)
		return ;
	while (xpnd_current && xpnd_current->prev)
		xpnd_current = xpnd_current->prev;
	while (xpnd_current)
	{
		printf("xpnd_current->substr[%d]: `%s`	;	xpnd_check: %d	;	str_to_join: `%s`\n", i, xpnd_current->substr, xpnd_current->xpnd_check, xpnd_current->str_to_join);
		i++;
		xpnd_current = xpnd_current->next;
	}
}

int	main(void) 	// ARRETE DOUBLIER QUE TU NE PEUX PAS UTILISER int ac, char **av PARCE QUE LE SHELL VA EXPAND LES $USER

{
	// char	*arg1="$USER\"$MAIL\"\'$PAGER\'$$$COL\"\"\"$$$ORTERM\"";
	char	*arg2 = "echo \"$Abba\'$Bebe\"\'$Coucou\"$Didier\'$Elephant\'\"$Fanny\'\"\'$Gold\'$Hi\"";
	// char	*arg3="\'$USER $USER\'$USER";
	// char *arg5="echo $$USER";

	// size_t	count = count_expand(arg1);
	// char **key;
	t_xpnd *xpnd_list = NULL;


	// key = char_keys(arg1, count);
	if (!tkn_xpnd_segmentation(arg2, &xpnd_list))
	return (1);

	// if (key)
	// {
	// 	for (size_t i = 0 ; i < count ; i++)
	// 	{
	// 		printf("arg: `%s`\nchar_keys: `%s`\n%td\n", arg1, key[i], count);
	// 		free(key[i]);
	// 	}
	// 	free(key);
	// }

	printf_xpnd(&xpnd_list);


	return (0);
}
