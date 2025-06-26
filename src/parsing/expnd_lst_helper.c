/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expnd_lst_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:32:35 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/26 15:10:52 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_xpnd	*create_filled_xpnd(char *src, size_t n, t_bool check, t_xpnd *q_curr)
{
	t_xpnd	*new_xpnd;

	new_xpnd = ft_lstnewxpnd();
	if (!new_xpnd)
		return (NULL);
	new_xpnd->substr = gc_strndup(src, n, GC_TKN);
	new_xpnd->xpnd_check = check;
	new_xpnd->in_single = q_curr->in_single;
	new_xpnd->in_double = q_curr->in_double;
	return (new_xpnd);
}

t_bool	is_variable_in_dquotes(char *s, size_t i, t_token *tkn)
{
	if (s[i] == '$' && s[i + 1]
		&& (s[i + 1] == '_'
			|| is_valid_keychar(s[i + 1]) || s[i + 1] == '?'
			|| s[i + 1] == '$')
		&& !heredoc_delim_check(tkn))
		return (true);
	return (false);
}
