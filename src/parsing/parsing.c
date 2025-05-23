/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:17:55 by tjacquel          #+#    #+#             */
/*   Updated: 2025/05/21 16:15:27 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_blank(int c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

// static int	is_operator(int c)
// {
// 	if (c == '|' || c == '<' || c == '>')
// 		return (1);
// 	return (0);
// }

int	lexer(char *prompt, t_token *token)
{
	size_t	i;
	size_t	j;
	size_t	n_dup;
	char	*value;

	i = 0;
	while (prompt[i])
	{
		if (prompt[i] == '\"')
		{
			j = i;
			while (prompt[j] != '\0' && prompt[j] != '\"')
			{
				j++;
			}
			n_dup = j - i;
			value = ft_strndup(prompt + i, n_dup);
			token = ft_lstnew_token(value, TOKEN_WORD, 2);
		}

		if (is_blank(prompt[i]))
			i++;
		i++;
	}
	// token->value = ft_strdup("bonjour");
	return (1);

}

int	parsing(char *prompt, t_shell *shell)
{
	(void)	shell;
	t_token	*token;

	token = NULL;
	// char *content = ft_strdup("bonjour");
	// token = ft_lstnew_token(content, 2, 3);
	// printf("%s\n", prompt);
	lexer(prompt, token);
	print_token(token);
	return (1);
}

