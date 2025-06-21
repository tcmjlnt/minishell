/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 18:20:41 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/21 11:47:15 by tjacquel         ###   ########.fr       */
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

// static int	key_exist(char *key, t_shell *shell)
// {

// 	char *value = get_env_value(shell->env, key); // non ca c'est juste remplacer la key par la value
// 	if (!value)
// 		return (NULL);
// 	// ca implique de decouper la string token_raw
// 	// char ** avec un ft_split, le separateur c'est ' ', '\t', '$'
// }

// static int	shall_i_expand(char	*str)
// {
// 	int i = 0;
// 	int in_single = 0;
// 	int in_double = 0;
// 	int	expand_res = 0;

// 	while (str[i])
// 	{
// 		if (str[i] == '\'' && !in_double) // je recontre une S_QUOTE et je NE suis PAS dans une D_QUOTE
// 		{
// 			in_single = !in_single;  // toggle in_single state
// 			expand_res = 0; // il faudrait split pour avoir la valeur key la plus petite possible
// 							// sinon $USER'$USER'"$USER" ca marche pas
// 		}
// 			 // toggle in_single state
// 		else if (str[i] == '\"' && !in_single) // je rencontre une D_QUOTE et je NE suis PAS dans une S_QUOTE
// 			in_double = !in_double; // toggle in_double state

// 	}
// }



/* cette fonction compte le nombre de KEYS potentielles*/
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

// char	*expand_dquotes(char *tkn_raw, t_shell *shell)
// {
// 	int	i = 0;
// 	size_t	len_noquotes = ft_strnlen_noquotes(tkn_raw);

// 	while (tkn_raw[i])
// 	{
// 		if (tkn_raw[i] == '$')
// 		{
// 			i++;
// 			while (is_valid_keychar(tkn_raw[i]))
// 			{
// 				// stocker chaque cle potentielle dans un char**??
// 				// faut peut etre split en amont ??
// 			}
// 		}
// 		// si ft_strcmp($KEY, cmt_delimiter??) == 0
// 	}
// }

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
			// if (substr[i + 1] == '_' || is_valid_keychar(substr[i + 1]) || substr[i + 1] == '?'|| substr[i + 1] == '$') // valid key start
			// {
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
			// }
			// else // invalid key start $9 $\ etc. MAIS AUSSI BLANKS
			// {
			// 	if (i > start)
			// 	{
			// 		new_xpnd = ft_lstnewxpnd();
			// 		if (!new_xpnd)
			// 			return (false);
			// 		new_xpnd = xpnd_new_fill(substr + start, i - start, false, xpnd_quotes_curr, new_xpnd);
			// 		ft_lstadd_back_xpnd(xpnd_list, new_xpnd);
			// 	}
			// 	i += 2;
			// 	start = i;
			// }
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
			// if (substr[i + 1] == '_' || is_valid_keychar(substr[i + 1]) || substr[i + 1] == '?') // || substr[i + 1] == '$') // Valid expansion key
			// {
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
			// }
			// else	// INVALID expansion key
			// {
			// 	if (i > start)
			// 	{
			// 		new_xpnd = ft_lstnewxpnd();
			// 		if (!new_xpnd)
			// 			return (false);
			// 		new_xpnd = xpnd_new_fill(substr + start, i - start, false, xpnd_quotes_curr, new_xpnd);
			// 		ft_lstadd_back_xpnd(xpnd_list, new_xpnd);
			// 	}
			// 	i += 2;
			// 	start = i;
			// }
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
	// char	*key_value;

	if (!xpnd_list || !(*xpnd_list))
		return (false);
	xpnd_curr = *xpnd_list;
	// key_value = NULL;

	while (xpnd_curr)
	{
		if (xpnd_curr->xpnd_check == true)
		{
			// key_value = get_env_value(env, xpnd_curr->substr);
			// if (!key_value)
			// 	return (false);
			// xpnd_curr->str_to_join = ft_strdup(key_value); // a check si faire comme ca avec
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
