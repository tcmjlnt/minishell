/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 18:20:41 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/26 11:07:20 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	squotes_scnd_segmentation(char *substr, t_xpnd *xpnd_quotes_curr,
								t_xpnd **xpnd_list)
{
	size_t	strlen;
	t_xpnd	*new_xpnd;

	strlen = ft_strlen(substr);
	new_xpnd = ft_lstnewxpnd();
	if (!new_xpnd)
		return (false);
	if (strlen >= 2)
	{
		new_xpnd = xpnd_new_fill(substr + 1, strlen - 2, false,
				xpnd_quotes_curr, new_xpnd);
		ft_lstadd_back_xpnd(xpnd_list, new_xpnd);
	}
	return (true);
}

int	dquotes_scnd_segmentation(char *substr, t_xpnd *xpnd_quotes_curr,
								t_xpnd **xpnd_list, t_token *tkn_curr)
{
	size_t	i;
	size_t	start;
	size_t	strlen;
	t_xpnd	*new_xpnd;

	strlen = ft_strlen(xpnd_quotes_curr->substr);
	if (strlen <= 2)
	{
		new_xpnd = ft_lstnewxpnd();
		if (!new_xpnd)
			return (false);
		new_xpnd = xpnd_new_fill("", 0, false, xpnd_quotes_curr, new_xpnd);
		ft_lstadd_back_xpnd(xpnd_list, new_xpnd);
		return (true);
	}
	i = 1;
	start = 1;
	while (i < strlen - 1)
	{
		if(substr[i] == '$' && (i + 1 < strlen - 1) && (substr[i + 1] == '_' || is_valid_keychar(substr[i + 1]) || substr[i + 1] == '?' || substr[i + 1] == '$') && !heredoc_delim_check(tkn_curr))
		{
			if (i > start)
			{
				new_xpnd = ft_lstnewxpnd();
				if (!new_xpnd)
					return (false);
				new_xpnd = xpnd_new_fill(substr + start, i - start, false, xpnd_quotes_curr, new_xpnd);
				ft_lstadd_back_xpnd(xpnd_list, new_xpnd);
			}
			start = ++i;
			if (substr[i] == '?' || substr[i] == '$' || ft_isdigit(substr[i]))
				i++;
			else
			{
				while (i < strlen - 1 && is_valid_keychar(substr[i]))
				i++;
			}

			new_xpnd = ft_lstnewxpnd();
			if (!new_xpnd)
				return (false);
			new_xpnd = xpnd_new_fill(substr + start, i - start, true, xpnd_quotes_curr, new_xpnd);
			ft_lstadd_back_xpnd(xpnd_list, new_xpnd);
			start = i;
		}
		else
			i++;
	}
	if (i > start)
	{
		new_xpnd = ft_lstnewxpnd();
		if (!new_xpnd)
			return (false);
		new_xpnd = xpnd_new_fill(substr + start, i - start, false, xpnd_quotes_curr, new_xpnd);
		ft_lstadd_back_xpnd(xpnd_list, new_xpnd);
	}
	return (true);
}

int	noquotes_scnd_segmentation(char *substr, t_xpnd *xpnd_quotes_curr, t_xpnd **xpnd_list, t_token *tkn_curr)
{
	size_t	i;
	size_t	start;
	t_xpnd	*new_xpnd;

	i = 0;
	start = 0;
	while(substr[i])
	{
		if(substr[i] == '$' && substr[i + 1] && (substr[i + 1] == '_' || is_valid_keychar(substr[i + 1]) || substr[i + 1] == '?' || substr[i + 1] == '$') && !heredoc_delim_check(tkn_curr))
		{
			if (i > start)
			{
				new_xpnd = ft_lstnewxpnd();
				if (!new_xpnd)
					return (false);
				new_xpnd = xpnd_new_fill(substr + start, i - start, false, xpnd_quotes_curr, new_xpnd);
				ft_lstadd_back_xpnd(xpnd_list, new_xpnd);
			}
			start = ++i;
			if (substr[i] == '?' || substr[i] == '$' || ft_isdigit(substr[i]))
				i++;
			else
			{
				while (substr[i] && is_valid_keychar(substr[i]))
					i++;
			}
			new_xpnd = ft_lstnewxpnd();
			if (!new_xpnd)
				return (false);
			new_xpnd = xpnd_new_fill(substr + start, i - start, true, xpnd_quotes_curr, new_xpnd);
			ft_lstadd_back_xpnd(xpnd_list, new_xpnd);
			start = i;
		}
		else
			i++;
	}
	if (i > start)
	{
		new_xpnd = ft_lstnewxpnd();
		if (!new_xpnd)
			return (false);
		new_xpnd = xpnd_new_fill(substr + start, i - start, false, xpnd_quotes_curr, new_xpnd);
		ft_lstadd_back_xpnd(xpnd_list, new_xpnd);
	}
	return (true);

}

int	scnd_segmentation(t_xpnd *xpnd_quotes_curr, t_xpnd **xpnd_list,
						t_token *tkn_current)
{
	if (!xpnd_quotes_curr || !xpnd_quotes_curr->substr)
		return (true);
	if (xpnd_quotes_curr->in_single)
	{
		if (!squotes_scnd_segmentation(xpnd_quotes_curr->substr,
				xpnd_quotes_curr, xpnd_list))
			return (false);
	}
	else if (xpnd_quotes_curr->in_double)
	{
		if (!dquotes_scnd_segmentation(xpnd_quotes_curr->substr,
				xpnd_quotes_curr, xpnd_list, tkn_current))
			return (false);
	}
	else
	{
		if (!noquotes_scnd_segmentation(xpnd_quotes_curr->substr,
				xpnd_quotes_curr, xpnd_list, tkn_current))
			return (false);
	}
	return (true);
}



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

int	handle_post_segmentation(t_token **tkn_xpnd_list, t_token *tkn_current,
								t_xpnd **xpnd_list, t_shell *shell)
{
	if (!handle_key_value(xpnd_list, shell))
		return (false);
	if (!handle_dollarsign_before_quotes(xpnd_list, tkn_current))
		return (false);
	// printf_xpnd(xpnd_list);
	if (!join_xpnd(xpnd_list, tkn_xpnd_list, tkn_current))
		return (false);
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
		// printf_xpnd(&xpnd_quotes_list);
		if (!scnd_segmentation_loop(xpnd_quotes_list, &xpnd_list, tkn_current))
			return (false);
		if (!handle_post_segmentation(tkn_xpnd_list, tkn_current, &xpnd_list,
				shell))
			return (false);
		tkn_current = tkn_current->next;
	}
	return (true);
}
