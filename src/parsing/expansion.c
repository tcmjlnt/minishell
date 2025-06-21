/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 18:20:41 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/21 13:19:29 by tjacquel         ###   ########.fr       */
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
	new_xpnd->substr = ft_strndup(src, n);
	new_xpnd->xpnd_check = xpnd_check;
	new_xpnd->in_single = xpnd_quotes_curr->in_single;
	new_xpnd->in_double = xpnd_quotes_curr->in_double;
	return (new_xpnd);
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

int	tkn_xpnd_segmentation2_dquotes(char *substr, t_xpnd *xpnd_quotes_curr, t_xpnd **xpnd_list)
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
		if(substr[i] == '$' && (i + 1 < strlen - 1) && (substr[i + 1] == '_' || is_valid_keychar(substr[i + 1]) || substr[i + 1] == '?' || substr[i + 1] == '$'))
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

int	tkn_xpnd_segmentation2_noquotes(char *substr, t_xpnd *xpnd_quotes_curr, t_xpnd **xpnd_list)
{
	size_t	i;
	size_t	start;
	t_xpnd	*new_xpnd;

	i = 0;
	start = 0;
	while(substr[i])
	{
		if(substr[i] == '$' && substr[i + 1] && (substr[i + 1] == '_' || is_valid_keychar(substr[i + 1]) || substr[i + 1] == '?' || substr[i + 1] == '$'))
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

int	tkn_xpnd_segmentation2(t_xpnd *xpnd_quotes_curr, t_xpnd **xpnd_list)
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
		if (!tkn_xpnd_segmentation2_dquotes(xpnd_quotes_curr->substr, xpnd_quotes_curr, xpnd_list))
			return (false);
	}
	else
	{
		if (!tkn_xpnd_segmentation2_noquotes(xpnd_quotes_curr->substr, xpnd_quotes_curr, xpnd_list))
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
				current_xpnd->substr = ft_strndup(tkn_raw + start, i - start);
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
			current_xpnd->substr = ft_strndup(tkn_raw + start, i - start);
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
		current_xpnd->substr = ft_strndup(tkn_raw + start, i - start);
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
				xpnd_curr->str_to_join = ft_itoa(shell->exit_status);
			else
				xpnd_curr->str_to_join = ft_strdup(get_env_value(shell->env, xpnd_curr->substr));

			if (!xpnd_curr->str_to_join)
				return (false);
		}
		else
		{
			xpnd_curr->str_to_join = ft_strdup(xpnd_curr->substr);
			if (!xpnd_curr->str_to_join)
				return (false);
		}
		xpnd_curr = xpnd_curr->next;
	}
	return (true);
}


int	check_empty_xpnd_node(t_xpnd **xpnd_list)
{
	t_xpnd	*xpnd_curr;
	int	keep_node;

	keep_node = 0;

	if (!xpnd_list || !(*xpnd_list))
		return (true);
	xpnd_curr = *xpnd_list;

	while(xpnd_curr)
	{
		if (xpnd_curr->str_to_join[0] != '\0' || xpnd_curr->in_double || xpnd_curr->in_single)
			keep_node++;
		xpnd_curr = xpnd_curr->next;
	}
	return (keep_node);

}

int	join_xpnd(t_xpnd **xpnd_list, t_token **tkn_xpnd_list, t_token *tkn_current)
{
	t_xpnd	*xpnd_curr;
	t_token	*tkn_xpnd_curr;
	char	*temp;
	char	*res;

	if (!xpnd_list || !(*xpnd_list))
		return (true);
	xpnd_curr = *xpnd_list;
	temp = ft_strdup(xpnd_curr->str_to_join);
	if (!temp)
		return (false);
	res = ft_strdup("");
	if (!res)
		return (false);
	if (xpnd_curr && !xpnd_curr->next)
	{
		res = ft_strdup(xpnd_curr->str_to_join);
		if (!res)
			return (false);
	}
	while (xpnd_curr && xpnd_curr->next) // besoin de check
	{
		// if (xpnd_curr->str_to_join[0] == '\0') // premiere tentative de skip le noeud vide -- ca segfault pour `echo $$USER`
		// 	xpnd_curr = xpnd_curr->next;
		res = ft_strjoin(temp, xpnd_curr->next->str_to_join);
		if (!res)
			return (false);
		temp = ft_strdup(res);
		if (!temp)
			return (false);
		xpnd_curr = xpnd_curr->next;
	}
	if (check_empty_xpnd_node(xpnd_list) != 0)
	//	|| (check_empty_xpnd_node(xpnd_list) == 0 && tkn_current && !tkn_current->next)) // pas sur en fait ! je veux pas passer empty string arg a l'exec ?
	{
		tkn_xpnd_curr = ft_lstnewtoken_xpnd();
		if (!tkn_xpnd_curr)
			return (false);
		tkn_xpnd_curr->token_raw = ft_strdup(tkn_current->token_raw);
		if (!tkn_current->token_raw)
			return (false);
		tkn_xpnd_curr->token_type = tkn_current->token_type;
		tkn_xpnd_curr->token_value = ft_strdup(res);
		if (!tkn_xpnd_curr->token_value)
			return (false);
		ft_lstadd_back_token(tkn_xpnd_list, tkn_xpnd_curr);
	}

	return (true);
}

int	handle_dollarsign_before_quotes(t_xpnd **xpnd_list)
{
	t_xpnd	*xpnd_curr;
	size_t	len;

	if (!xpnd_list || !(*xpnd_list))
		return (false);
	xpnd_curr = *xpnd_list;
	while (xpnd_curr)
	{
		len = ft_strlen(xpnd_curr->str_to_join);
		if (xpnd_curr->str_to_join[len - 1] == '$' && !xpnd_curr->xpnd_check && xpnd_curr->next
			&& (xpnd_curr->next->in_single || xpnd_curr->next->in_double) && !xpnd_curr->in_double
			&& !xpnd_curr->in_single)
		{
			xpnd_curr->str_to_join = ft_strndup(xpnd_curr->str_to_join, len - 1);
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
			if (!tkn_xpnd_segmentation2(temp_xpnd_quotes, &xpnd_list))
				return (false);


			temp_xpnd_quotes = temp_xpnd_quotes->next;
		}
		// printf("================= ENTERING XPND LIST 		  FROM TOKEN[%d] PRINTF =================\n", token_index);
		// printf_xpnd(&xpnd_list);

		if (!handle_key_value(&xpnd_list, shell))
			return (false);
		// printf("================= ENTERING XPND LIST W/ KEY_VALUE FROM TOKEN[%d] PRINTF =================\n", token_index);
		// printf_xpnd(&xpnd_list);
		if (!handle_dollarsign_before_quotes(&xpnd_list))
			return (false);
		if (!join_xpnd(&xpnd_list, tkn_xpnd_list, tkn_current))
			return (false);

		free_t_xpnd_list(xpnd_quotes_list);
		free_t_xpnd_list(xpnd_list);


		token_index++;
		tkn_current = tkn_current->next;
	}
	return (true);
}
