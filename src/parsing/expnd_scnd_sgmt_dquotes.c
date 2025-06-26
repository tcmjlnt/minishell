/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expnd_scnd_sgmt_dquotes.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 13:58:39 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/26 14:01:09 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	create_text_before_var(char *sub, t_dq_state *st,
								t_xpnd **list, t_xpnd *q_curr)
{
	t_xpnd	*new_xpnd;

	if (st->i > st->start)
	{
		new_xpnd = create_filled_xpnd(sub + st->start, st->i - st->start,
				false, q_curr);
		if (!new_xpnd)
			return (false);
		ft_lstadd_back_xpnd(list, new_xpnd);
	}
	st->start = ++st->i;
	return (true);
}

static int	create_variable_segment(char *sub, t_dq_state *st,
								t_xpnd **list, t_xpnd *q_curr)
{
	t_xpnd	*new_xpnd;

	if (sub[st->i] == '?' || sub[st->i] == '$' || ft_isdigit(sub[st->i]))
		st->i++;
	else
	{
		while (st->i < st->strlen - 1 && is_valid_keychar(sub[st->i]))
			st->i++;
	}
	new_xpnd = create_filled_xpnd(sub + st->start, st->i - st->start, true,
			q_curr);
	if (!new_xpnd)
		return (false);
	ft_lstadd_back_xpnd(list, new_xpnd);
	st->start = st->i;
	return (true);
}

static int	handle_dquote_variable(char *sub, t_dq_state *st,
								t_xpnd **list, t_xpnd *q_curr)
{
	if (!create_text_before_var(sub, st, list, q_curr))
		return (false);
	if (!create_variable_segment(sub, st, list, q_curr))
		return (false);
	return (true);
}

static int	init_dquote_state(t_dq_state *st, t_xpnd *xpnd_quotes_curr)
{
	st->strlen = ft_strlen(xpnd_quotes_curr->substr);
	if (st->strlen < 2)
		return (0);
	st->i = 1;
	st->start = 1;
	return (1);
}

int	dquotes_scnd_segmentation(char *substr, t_xpnd *xpnd_quotes_curr,
								t_xpnd **xpnd_list, t_token *tkn_curr)
{
	t_dq_state	st;
	t_xpnd		*new_xpnd;

	if (!init_dquote_state(&st, xpnd_quotes_curr))
		return (true);
	while (st.i < st.strlen - 1)
	{
		if (is_variable_in_dquotes(substr, st.i, tkn_curr))
		{
			if (!handle_dquote_variable(substr, &st, xpnd_list,
					xpnd_quotes_curr))
				return (false);
		}
		else
			st.i++;
	}
	if (st.i > st.start)
	{
		new_xpnd = create_filled_xpnd(substr + st.start, st.i - st.start,
				false, xpnd_quotes_curr);
		if (!new_xpnd)
			return (false);
		ft_lstadd_back_xpnd(xpnd_list, new_xpnd);
	}
	return (true);
}
