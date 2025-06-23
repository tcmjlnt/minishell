/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 18:20:41 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/23 15:52:49 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int is_valid_keychar(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	else
		return (0);
}

t_xpnd	*xpnd_new_fill(char	*src, size_t n, t_bool xpnd_check, t_xpnd *xpnd_quotes_curr, t_xpnd *new_xpnd)
{
	new_xpnd->substr = gc_strndup(src, n, GC_TKN);
	new_xpnd->xpnd_check = xpnd_check;
	new_xpnd->in_single = xpnd_quotes_curr->in_single;
	new_xpnd->in_double = xpnd_quotes_curr->in_double;
	return (new_xpnd);
}

int	heredoc_delim_check(t_token *tkn_curr)
{
	if (tkn_curr->prev && tkn_curr->prev->token_type == TOKEN_REDIRECT_HEREDOC)
		return (true);
	return (false);
}

int	redir_prev_tkn_check(t_token *tkn_curr)
{
	if (tkn_curr->prev && (tkn_curr->prev->token_type == TOKEN_REDIRECT_APPEND
		|| tkn_curr->prev->token_type == TOKEN_REDIRECT_IN || tkn_curr->prev->token_type == TOKEN_REDIRECT_OUT))
		return (true);
	return (false);
}

int	tkn_xpnd_segmentation2_squotes(char *substr, t_xpnd *xpnd_quotes_curr, t_xpnd **xpnd_list) // gere les subtokens single quotes
{
	size_t	strlen;
	t_xpnd	*new_xpnd;

	strlen = ft_strlen(substr);
	new_xpnd = ft_lstnewxpnd();
	if (!new_xpnd)
		return (false);
	if (strlen >= 2)
	{
		new_xpnd = xpnd_new_fill(substr + 1, strlen - 2, false, xpnd_quotes_curr, new_xpnd);
		ft_lstadd_back_xpnd(xpnd_list, new_xpnd);
	}
	return (true);
}

int	tkn_xpnd_segmentation2_dquotes(char *substr, t_xpnd *xpnd_quotes_curr, t_xpnd **xpnd_list, t_token *tkn_curr)
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

int	tkn_xpnd_segmentation2_noquotes(char *substr, t_xpnd *xpnd_quotes_curr, t_xpnd **xpnd_list, t_token *tkn_curr)
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

int	tkn_xpnd_segmentation2(t_xpnd *xpnd_quotes_curr, t_xpnd **xpnd_list, t_token *tkn_current)
{
	if (!xpnd_quotes_curr || !xpnd_quotes_curr->substr)
		return (true);
	if (xpnd_quotes_curr->in_single)
	{
		if (!tkn_xpnd_segmentation2_squotes(xpnd_quotes_curr->substr, xpnd_quotes_curr, xpnd_list))
			return (false);
	}
	else if (xpnd_quotes_curr->in_double)
	{
		if (!tkn_xpnd_segmentation2_dquotes(xpnd_quotes_curr->substr, xpnd_quotes_curr, xpnd_list, tkn_current))
			return (false);
	}
	else
	{
		if (!tkn_xpnd_segmentation2_noquotes(xpnd_quotes_curr->substr, xpnd_quotes_curr, xpnd_list, tkn_current))
			return (false);
	}
	return (true);
}

int	tkn_xpnd_quotes_segmentation(char *tkn_raw, t_xpnd **xpnd_list)
{
	size_t	i;
	size_t	start;
	t_bool	in_single;
	t_bool	in_double;

	t_xpnd	*current_xpnd;


	i = 0;
	start = 0;
	in_double = false;
	in_single = false;
	while (tkn_raw[i])
	{
		if ((tkn_raw[i] == '\'' && !in_double) || (tkn_raw[i] == '\"' && !in_single))
		{
			if (i > start)
			{
				current_xpnd = ft_lstnewxpnd();
				if (!current_xpnd)
					return (false);
				current_xpnd->substr = gc_strndup(tkn_raw + start, i - start, GC_TKN);
				current_xpnd->xpnd_check = false;
				ft_lstadd_back_xpnd(xpnd_list, current_xpnd);
			}
			if (tkn_raw[i] == '\'' && !in_double)
				in_single = !in_single;
			else if (tkn_raw[i] == '\"' && !in_single)
				in_double = !in_double;
			start = i;
			i++;
			while(tkn_raw[i])
			{
				if ((tkn_raw[i] == '\'' && in_single && !in_double) ||
					(tkn_raw[i] == '\"' && in_double && !in_single))
				{
					current_xpnd = ft_lstnewxpnd();
					if (!current_xpnd)
						return (false);
					current_xpnd->in_single = in_single;
					current_xpnd->in_double = in_double;
					if (tkn_raw[i] == '\'' && !in_double)
						in_single = !in_single;
					else if (tkn_raw[i] == '\"' && !in_single)
						in_double = !in_double;
					i++;
					break ;
				}
				i++;
			}
			current_xpnd->substr = gc_strndup(tkn_raw + start, i - start, GC_TKN);
			current_xpnd->xpnd_check = false;
			ft_lstadd_back_xpnd(xpnd_list, current_xpnd);
			start = i;
		}
		else
			i++;
	}
	if (i > start) {
		current_xpnd = ft_lstnewxpnd();
		if (!current_xpnd)
			return (false);
		current_xpnd->substr = gc_strndup(tkn_raw + start, i - start, GC_TKN);
		current_xpnd->xpnd_check = false;
		ft_lstadd_back_xpnd(xpnd_list, current_xpnd);
	}
	return (true);
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
		printf("xpnd_current->substr[%d]: `%s`	;	in_single: %d	;	in_double: %d	;	xpnd_check: %d	;	str_to_join: `%s`\n",
			 i, xpnd_current->substr, xpnd_current->in_single, xpnd_current->in_double, xpnd_current->xpnd_check, xpnd_current->str_to_join);
		i++;
		xpnd_current = xpnd_current->next;
	}
}

int	handle_key_value(t_xpnd **xpnd_list, t_shell *shell)
{
	t_xpnd	*xpnd_curr;

	if (!xpnd_list || !(*xpnd_list))
		return (false);
	xpnd_curr = *xpnd_list;

	while (xpnd_curr)
	{
		if (xpnd_curr->xpnd_check == true)
		{
			if (ft_strcmp(xpnd_curr->substr, "?") == 0)
				xpnd_curr->str_to_join = gc_itoa(shell->exit_status, GC_TKN);
			else
				xpnd_curr->str_to_join = gc_strdup(get_env_value(shell->env, xpnd_curr->substr, GC_TKN), GC_TKN);

			if (!xpnd_curr->str_to_join)
				return (false);
		}
		else
		{
			xpnd_curr->str_to_join = gc_strdup(xpnd_curr->substr, GC_TKN);
			if (!xpnd_curr->str_to_join)
				return (false);
		}
		xpnd_curr = xpnd_curr->next;
	}
	return (true);
}


int	check_empty_xpnd_node(t_xpnd **xpnd_list, t_token *tkn_curr)
{
	t_xpnd	*xpnd_curr;
	int	keep_node;

	keep_node = 0;

	if (!xpnd_list || !(*xpnd_list))
		return (true);
	xpnd_curr = *xpnd_list;

	while(xpnd_curr)
	{
		if (xpnd_curr->str_to_join[0] != '\0' || xpnd_curr->in_double || xpnd_curr->in_single
			|| redir_prev_tkn_check(tkn_curr) != 0)
			keep_node++;
		xpnd_curr = xpnd_curr->next;
	}
	return (keep_node);

}

int	join_xpnd(t_xpnd **xpnd_list, t_token **tkn_xpnd_list, t_token *tkn_current)
{
	t_xpnd	*xpnd_curr;
	t_token	*tkn_xpnd_curr;
	char	*res;
	char	*temp_join;
	int		i;
	int		start;

	i = 0;
	start = 0;
	if (!xpnd_list || !(*xpnd_list))
		return (true);
	xpnd_curr = *xpnd_list;
	res = gc_strdup("", GC_TKN);
	if (!res)
		return (false);
	while (xpnd_curr)
	{
		temp_join = gc_strjoin(res, xpnd_curr->str_to_join, GC_TKN);
		// free(res);
		if (!temp_join)
			return (false);
		res = temp_join;
		xpnd_curr = xpnd_curr->next;
	}
	if (check_empty_xpnd_node(xpnd_list, tkn_current) != 0)
	{
		tkn_xpnd_curr = ft_lstnewtoken_xpnd();
		if (!tkn_xpnd_curr)
		{
			// free(res);
			return (false);
		}
		tkn_xpnd_curr->token_raw = gc_strdup(tkn_current->token_raw, GC_TKN);
		if (!tkn_xpnd_curr->token_raw)
		{
			// free(res);
			// free(tkn_xpnd_curr);
			return (false);
		}
		tkn_xpnd_curr->token_type = tkn_current->token_type;
		// if (!xpnd_curr->in_double && !xpnd_curr->in_single && tkn_current->prev && !is_redir_operator(tkn_current->prev))
		// {
		// 	while (res[i])
		// 	{
		// 		start = i;
		// 		while (res[i] && is)
		// 	}
		// }
		tkn_xpnd_curr->token_value = gc_strdup(res, GC_TKN);
		if (!tkn_xpnd_curr->token_value)
		{
			// free(res);
			// free(tkn_xpnd_curr->token_raw);
			// free(tkn_xpnd_curr);
			return (false);
		}
		ft_lstadd_back_token(tkn_xpnd_list, tkn_xpnd_curr);
	}
	// free (res);
	return (true);
}

int	trailing_dollar_count(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '$')
			count++;
		else
			count = 0;
		i++;
	}
	return (count);
}

int	handle_dollarsign_before_quotes(t_xpnd **xpnd_list, t_token *tkn_current)
{
	t_xpnd	*xpnd_curr;
	size_t	len;

	if (!xpnd_list || !(*xpnd_list))
		return (false);
	xpnd_curr = *xpnd_list;
	while (xpnd_curr)
	{
		len = ft_strlen(xpnd_curr->str_to_join);
		if (len > 0 && xpnd_curr->str_to_join[len - 1] == '$' && !xpnd_curr->xpnd_check && xpnd_curr->next
			&& (xpnd_curr->next->in_single || xpnd_curr->next->in_double) && !xpnd_curr->in_double
			&& !xpnd_curr->in_single)
		{
			if (heredoc_delim_check(tkn_current) && xpnd_curr->str_to_join[len - 2] && xpnd_curr->str_to_join[len - 2] == '$' && trailing_dollar_count(xpnd_curr->str_to_join) % 2 == 0) // SKIP 1 $ si nombre de $ impair -- AUCUN SKIP SI PAIR
				xpnd_curr->str_to_join = gc_strndup(xpnd_curr->str_to_join, len, GC_TKN);
			else
				xpnd_curr->str_to_join = gc_strndup(xpnd_curr->str_to_join, len - 1, GC_TKN);
			if (!xpnd_curr->str_to_join)
				return (false);
		}
		xpnd_curr = xpnd_curr->next;
	}
	return (true);

}

int	handle_expansion(t_token **tkn_list, t_token **tkn_xpnd_list, t_shell *shell)
{
	t_token	*tkn_current;
	t_xpnd	*xpnd_quotes_list;
	t_xpnd	*xpnd_list;
	t_xpnd	*temp_xpnd_quotes;
	int		token_index = 0;


	if (!tkn_list || !(*tkn_list))
		return (false);
	tkn_current = *tkn_list;
	while (tkn_current && tkn_current->prev)
		tkn_current = tkn_current->prev;

	while (tkn_current)
	{
		xpnd_quotes_list = NULL;
		xpnd_list = NULL;
		if (!tkn_xpnd_quotes_segmentation(tkn_current->token_raw, &xpnd_quotes_list))
			return (false);
		// printf("================= ENTERING XPND QUOTES LIST 	  FROM TOKEN[%d] PRINTF =================\n", token_index);
		// printf_xpnd(&xpnd_quotes_list);
		temp_xpnd_quotes = xpnd_quotes_list;
		while (temp_xpnd_quotes)
		{
			if (!tkn_xpnd_segmentation2(temp_xpnd_quotes, &xpnd_list, tkn_current))
				return (false);

			temp_xpnd_quotes = temp_xpnd_quotes->next;
		}
		// printf("================= ENTERING XPND LIST 		  FROM TOKEN[%d] PRINTF =================\n", token_index);
		// printf_xpnd(&xpnd_list);
		if (!handle_key_value(&xpnd_list, shell))
			return (false);
		// printf("================= ENTERING XPND LIST W/ KEY_VALUE FROM TOKEN[%d] PRINTF =================\n", token_index);
		// printf_xpnd(&xpnd_list);
		if (!handle_dollarsign_before_quotes(&xpnd_list, tkn_current))
			return (false);
		if (!join_xpnd(&xpnd_list, tkn_xpnd_list, tkn_current))
			return (false);

		// free_t_xpnd_list(xpnd_quotes_list);
		// free_t_xpnd_list(xpnd_list);


		token_index++;
		tkn_current = tkn_current->next;
	}
	return (true);
}
