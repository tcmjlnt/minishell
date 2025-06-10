/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 18:20:41 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/10 18:56:55 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_it_a_valid_key(char *buff)
{
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

char	*expand_dquotes(t_token *tkn_current, t_shell *shell)
{
	int	i = 0;
	size_t	len_noquotes = ft_strlen_noquotes(tkn_current->token_raw);

	while (tkn_current->token_raw[i])
	{

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
		if (is_inside_dquotes(tkn_current->token_raw) && ft_strchr(tkn_current, '$'))
		{
			tkn_current->token_value = expand_dquotes(tkn_current, shell);
		}
	}
}