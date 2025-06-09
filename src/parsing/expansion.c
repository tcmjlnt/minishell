/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 18:20:41 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/09 17:04:40 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*expand_dquotes(t_token *current_token, t_shell *shell)
{
	int	i = 0;
	size_t	len_noquotes = ft_strlen_noquotes(current_token->token_raw);

	while (current_token->token_raw[i])
	{

		// si ft_strcmp($KEY, cmt_delimiter??) == 0
	}
}

int	handle_expansion(t_token **token_list_head, t_shell *shell)
{
	t_token	*current_token;

	if (!token_list_head || !(*token_list_head))
		return (false);
	while (current_token && current_token->prev)
		current_token = current_token->prev;
	while (current_token)
	{
		if (is_inside_dquotes(current_token->token_raw) && ft_strchr(current_token, '$'))
		{
			current_token->token_value = expand_dquotes(current_token, shell);
		}
	}
}