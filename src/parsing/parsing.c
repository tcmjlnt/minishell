/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:17:55 by tjacquel          #+#    #+#             */
/*   Updated: 2025/05/27 21:53:33 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_quote_enclosed(char *prompt)
{
	int i = 0;
	int in_single = 0;
	int in_double = 0;

	while (prompt[i])
	{
		if (prompt[i] == '\'' && !in_double)
			in_single = !in_single; // toggle in_single state
		else if (prompt[i] == '\"' && !in_single)
			in_double = !in_double; // toggle in_double state
		i++;
	}
	return (in_single == 0 && in_double == 0);
}

int first_syntax_check(char *prompt)
{

	int i = 0;
	if (!is_quote_enclosed(prompt))
	{
		printf("Unclosed quotes\n");
		return (false);
	}
	// while(prompt[i])
	// {
	// 	if (!is_quote_enclosed(prompt))
	// 		return (false);
	// 	if(prompt[i] = '\'')
	// 	{
	// 		printf("Syntax error %c\n", prompt[i]);
	// 		return false;
	// 	}
	// 	i++;
	// }

	i = 0;
	while(prompt[i])
	{
		if(prompt[i + 1] && (prompt[i] == '|' && prompt[i + 1] == '|'))
		{
			printf("Syntax error %c\n", prompt[i]);
			return false;
		}
		i++;
	}
	i = 0;
	while(prompt[i])
	{
		if(prompt[i + 1] && (prompt[i] == '&' && prompt[i + 1] == '&'))
		{
			printf("Syntax error %c\n", prompt[i]);
			return false;
		}
		i++;
	}
	return (true);
}

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
	// t_token *current = token;


	// while(prompt[i] != "|")
	// {
	// 	current->args = ft_split(prompt, " ");
	// 	// args[0] = cmd;
	// 	// args[0+i] = args;

	// }
	// token->next

	while (prompt[i] != '\"' || prompt[i] != '\'')
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
	if (!first_syntax_check(prompt))
	{
		return (false);
	}
	// char *content = ft_strdup("bonjour");
	// token = ft_lstnew_token(content, 2, 3);
	printf("%s\n", prompt);
	// lexer(prompt, token);
	print_token(token);
	return (1);
}



