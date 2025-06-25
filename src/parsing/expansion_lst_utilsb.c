/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_lst_utilsb.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 22:02:28 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/25 22:08:04 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_xpnd	*xpnd_new_fill(char	*src, size_t n, t_bool xpnd_check, t_xpnd *xpnd_quotes_curr, t_xpnd *new_xpnd)
{
	new_xpnd->substr = gc_strndup(src, n, GC_TKN);
	new_xpnd->xpnd_check = xpnd_check;
	new_xpnd->in_single = xpnd_quotes_curr->in_single;
	new_xpnd->in_double = xpnd_quotes_curr->in_double;
	return (new_xpnd);
}