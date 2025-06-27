/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:33:13 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/27 15:45:20 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	process_key_value(t_xpnd *xpnd_curr, t_shell *shell)
{
	if (xpnd_curr->xpnd_check == true)
	{
		if (ft_strcmp(xpnd_curr->substr, "?") == 0)
			xpnd_curr->str_to_join = gc_itoa(shell->exit_status, GC_TKN);
		else
			xpnd_curr->str_to_join = gc_strdup(get_env_value(shell->env,
						xpnd_curr->substr, GC_TKN), GC_TKN);
		if (!xpnd_curr->str_to_join)
			return (false);
	}
	else
	{
		xpnd_curr->str_to_join = gc_strdup(xpnd_curr->substr, GC_TKN);
		if (!xpnd_curr->str_to_join)
			return (false);
	}
	return (true);
}

int	handle_key_value(t_xpnd **xpnd_list, t_shell *shell)
{
	t_xpnd	*xpnd_curr;

	if (!xpnd_list || !(*xpnd_list))
		return (false);
	xpnd_curr = *xpnd_list;
	while (xpnd_curr)
	{
		if (!process_key_value(xpnd_curr, shell))
			return (false);
		xpnd_curr = xpnd_curr->next;
	}
	return (true);
}

int	scnd_segmentation_loop(t_xpnd *xpnd_quotes_list, t_xpnd **xpnd_list,
							t_token *tkn_current)
{
	t_xpnd	*temp_xpnd_quotes;

	if (!xpnd_quotes_list)
		return (false);
	temp_xpnd_quotes = xpnd_quotes_list;
	while (temp_xpnd_quotes)
	{
		if (!scnd_segmentation(temp_xpnd_quotes, xpnd_list, tkn_current))
			return (false);
		temp_xpnd_quotes = temp_xpnd_quotes->next;
	}
	return (true);
}

static int	handle_post_segmentation(t_token **tkn_xpnd_list, t_token *tkn_current,
								t_xpnd **xpnd_list, t_shell *shell)
{
	if (!handle_key_value(xpnd_list, shell))
	{
		// printf("je sors a handle_key_value\n");
		return (false);
	}
	// printf("-------------------- AFTER HANDLE KEY VALUE TOKEN[%d] ---------------------\n", *i);
	// printf_xpnd(xpnd_list);
	if (!handle_dollarsign_before_quotes(xpnd_list, tkn_current))
		return (false);
	if (!join_xpnd(xpnd_list, tkn_xpnd_list, tkn_current))
	{
		// printf("je sors a join_xpnd\n");
		return (false);
	}
	return (true);
}

int	handle_expansion(t_token **tkn_list, t_token **tkn_xpnd_list,
						t_shell *shell)
{
	t_token	*tkn_current;
	t_xpnd	*xpnd_quotes_list;
	t_xpnd	*xpnd_list;

	if (!tkn_list || !(*tkn_list))
		return (false);
	tkn_current = *tkn_list;
	while (tkn_current)
	{
		xpnd_quotes_list = NULL;
		xpnd_list = NULL;
		if (!quotes_first_segmentation(tkn_current->token_raw,
				&xpnd_quotes_list))
			return (false);
		// printf("-------------------- AFTER FIRST SEGMENTATION TOKEN[%d]----------------------\n", i);
		// printf_xpnd(&xpnd_quotes_list);
		if (!scnd_segmentation_loop(xpnd_quotes_list, &xpnd_list, tkn_current))
		{
			printf("je sors a la sncd_segmentation_loop\n");
			return (false);
		}
		// printf("-------------------- AFTER SCND SEGMENTATION TOKEN[%d] ---------------------\n", i);
		// printf_xpnd(&xpnd_list);

		if (!handle_post_segmentation(tkn_xpnd_list, tkn_current, &xpnd_list,
				shell))
		{
			// printf("je sors a la handle_post_segmentation\n");
			return (false);
		}
		tkn_current = tkn_current->next;
	}
	return (true);
}
