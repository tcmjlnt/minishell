/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:17:55 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/26 17:49:26 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_args(t_cmd *cmd)
{
	int		i ;
	int		j;
	t_cmd	*temp;

	i = 0;
	j = 0;
	temp = cmd;
	while (temp && temp->prev)
		temp = temp->prev;
	while (temp)
	{
		printf("-----------------\nCommand %d: `%s` ", i, temp->cmd);
		printf("; is_builtin=%d\n", temp->is_builtin);
		if (temp->args)
		{
			j = 0;
			while (temp->args[j++])
				printf("arg[%d]: `%s`\n", j, temp->args[j]);
		}
		if (temp->redir)
			print_redir(temp);
		i++;
		temp = temp->next;
	}
	printf("----------------- END OF COMMAND LIST -----------------\n");
}

static int	token_operator(char *prompt, int *i, t_token **tkn_list)
{
	int	res;

	res = token_append(prompt, i, tkn_list);
	if (res != false)
		return (res);
	res = token_heredoc(prompt, i, tkn_list);
	if (res != false)
		return (res);
	res = token_pipe(prompt, i, tkn_list);
	if (res != false)
		return (res);
	res = token_out(prompt, i, tkn_list);
	if (res != false)
		return (res);
	res = token_in(prompt, i, tkn_list);
	if (res != false)
		return (res);
	return (false);
}

static int	token_word(char *prompt, int *i, t_token **tkn_list)
{
	int		start;
	t_token	*token;

	start = (*i);
	while (prompt[(*i)])
	{
		if ((is_blank(prompt[(*i)]) || is_operator_char(prompt[(*i)]))
			&& !is_inside_quotes(prompt, *i))
			break ;
		(*i)++;
	}
	token = ft_lstnewtoken(prompt + start, (*i) - start, TKN_WORD);
	if (!token)
		return (false);
	ft_lstadd_back_token(tkn_list, token);
	return (true);
}

int	tokenize_prompt(char *prompt, t_token **tkn_list)
{
	int	i;
	int	res;

	i = 0;
	while (prompt[i])
	{
		while (prompt[i] && is_blank(prompt[i]))
			i++;
		if (!prompt[i])
			break ;
		res = token_operator(prompt, &i, tkn_list);
		if (res == -1)
			return (false);
		if (res == true)
			continue ;
		if (!token_word(prompt, &i, tkn_list))
			return (false);
	}
	return (true);
}

int	parsing(char *prompt, t_cmd **cmd_list, t_shell *shell)
{
	t_token	*tkn_list;
	t_token	*tkn_xpnd_list;

	tkn_list = NULL;
	tkn_xpnd_list = NULL;
	if (!first_syntax_check(prompt, shell))
		return (false);
	if (!tokenize_prompt(prompt, &tkn_list))
		return (false);
	if (!check_token(&tkn_list))
		return (false);
	if (!handle_expansion(&tkn_list, &tkn_xpnd_list, shell))
		return (false);
	if (!parse_tokens(cmd_list, &tkn_xpnd_list, shell))
		return (false);
	return (1);
}
