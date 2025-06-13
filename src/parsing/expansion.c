/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 18:20:41 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/13 19:50:39 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int is_valid_keychar(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	else
		return (0);
}

static int	key_exist(char *key, t_shell *shell)
{

	char *value = get_env_value(shell->env, key); // non ca c'est juste remplacer la key par la value
	if (!value)
		return (NULL);
	// ca implique de decouper la string token_raw
	// char ** avec un ft_split, le separateur c'est ' ', '\t', '$'
}

static int	shall_i_expand(char	*str)
{
	int i = 0;
	int in_single = 0;
	int in_double = 0;
	int	expand_res = 0;

	while (str[i])
	{
		if (str[i] == '\'' && !in_double) // je recontre une S_QUOTE et je NE suis PAS dans une D_QUOTE
		{
			in_single = !in_single;  // toggle in_single state
			expand_res = 0; // il faudrait split pour avoir la valeur key la plus petite possible
							// sinon $USER'$USER'"$USER" ca marche pas
		}
			 // toggle in_single state
		else if (str[i] == '\"' && !in_single) // je rencontre une D_QUOTE et je NE suis PAS dans une S_QUOTE
			in_double = !in_double; // toggle in_double state

	}
}

	whatwhedoinghere(char *tkn_raw, size_t i)
{
	// on veut faire quoi ? on veut juste la longueur de la string expand ?
}

	char_keys(char *tkn_raw, size_t count_expand)
{
	size_t	i;
	size_t	start;
	size_t	count_keys;
	t_bool	in_single;
	t_bool	in_double;

	i = 0;
	start = 0;
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
			char *key = ft_strndup(tkn_raw + start, i);
			if (!tkn_raw[i])
				break ;
		}
		i++;
	}
	return (count_expand);
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
		i++;
	}
	return (count_expand);
}

char	*expand_dquotes(char *tkn_raw, t_shell *shell)
{
	int	i = 0;
	size_t	len_noquotes = ft_strlen_noquotes(tkn_raw);

	while (tkn_raw[i])
	{
		if (tkn_raw[i] == '$')
		{
			i++;
			while (is_valid_keychar(tkn_raw[i]))
			{
				// stocker chaque cle potentielle dans un char**??
				// faut peut etre split en amont ??
			}
		}
		// si ft_strcmp($KEY, cmt_delimiter??) == 0
	}
}

int	handle_expansion(t_token **tkn_list, t_shell *shell)
{
	t_token	*tkn_current;

	if (!tkn_list || !(*tkn_list))
		return (false);
	while (tkn_current && tkn_current->prev)
		tkn_current = tkn_current->prev;
	while (tkn_current)
	{
		if (is_inside_dquotes(tkn_current->token_raw) && ft_strchr(tkn_current->token_raw, '$'))
		{
			tkn_current->token_value = expand_dquotes(tkn_current, shell);
			if (key_exist(tkn_current->token_raw))
		}
	}
}