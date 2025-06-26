/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:17:55 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/26 16:27:19 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_operator(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (true);
	else
		return (false);
}



int	closed_quotes(char *prompt)
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


static int	is_inside_quotes(char *prompt, int pos)
{
	int	i = 0;
	int	in_single = 0;
	int	in_double = 0;

	while (i < pos && prompt[i])
	{
		if (prompt[i] == '\'' && !in_double)
			in_single = !in_single; // toggle in_single state
		else if (prompt[i] == '\"' && !in_single)
			in_double = !in_double; // toggle in_double state
		i++;
	}
	return (in_single || in_double);
}

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_blank(int c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

int	pipes_check(char *prompt)
{
	int	i;

	i = 0;
	while (prompt[i] && is_blank(prompt[i]))
		i++;
	if (prompt[i] == '|' && !is_inside_quotes(prompt, i))
	{
		printf("minishell: syntax error near unexpected token begin `|'\n");
		return false;
	}
	i = 0;
	while (prompt[i])
	{
		if (prompt[i] == '|' && !is_inside_quotes(prompt, i))
		{
			if (prompt[i + 1] && prompt[i + 1] == '|')
			{
				printf("minishell: syntax error near unexpected token `|'\n");
				return false;
			}
			i++;
			while (prompt[i] && is_blank(prompt[i]))
				i++;
			if (prompt[i] == '|')
			{
				printf("Syntax error %c\n", prompt[i]);
				return false;
			}
			if (!prompt[i])
			{
				printf("Syntax error: pipe at the end\n");
				return false;
			}
			continue ;
		}
		i++;
	}

	return (true);
}

int	first_syntax_check(char *prompt, t_shell *shell)
{

	if (!closed_quotes(prompt))
	{
		printf("Unclosed quotes\n");
		shell->exit_status = 2;
		return (false);
	}

	if (!pipes_check(prompt))
		return (false);
	return (true);
}

void	print_redir(t_cmd *temp)
{
	t_redir	*redir_current = NULL;
	int	i = 0;

	if (!temp)
		return ;
	redir_current = temp->redir;
	while (redir_current)
	{
		printf("	redir_file[%d]: `%s`	;	redir_delim[%d]: `%s`	;	redir_type[%d]: `%d`\n", i, redir_current->file, i, redir_current->delim, i, redir_current->type);
		i++;
		redir_current = redir_current->next;
	}
}

void	print_args(t_cmd *cmd)
{
	int	i ;
	int	j;
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

// void	first_lexing

t_token	*token_wordtpye(char *prompt, int *i)
{
	int		start;
	t_token	*temp;
	// int i = 0;
	// int in_single = 0;
	// int in_double = 0;

	// while (i < pos && prompt[i])
	// {
	// 	if (prompt[i] == '\'' && !in_double)
	// 		in_single = !in_single; // toggle in_single state
	// 	else if (prompt[i] == '\"' && !in_single)
	// 		in_double = !in_double; // toggle in_double state
	// 	i++;
	// }

	start = 0;
	start = (*i);
	while (prompt[(*i)]) // && !is_blank(prompt[(*i)]) && !is_operator(prompt[(*i)]))
	{
		if ((is_blank(prompt[(*i)]) || is_operator(prompt[(*i)])) && !is_inside_quotes(prompt, *i))
			break ;
		else
			(*i)++;

	}
	temp = ft_lstnewtoken(prompt + start, (*i) - start, TKN_WORD);
	if (!temp)
		return (NULL);

	return (temp);

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
		if ((is_blank(prompt[(*i)]) || is_operator(prompt[(*i)]))
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
	int res;

	i = 0;
	while (prompt[i])
	{
		while (prompt[i] && is_blank(prompt[i]))
			i++;
		if (!prompt[i])
			break ;
		res = token_operator(prompt, &i, tkn_list);
		if (res == -1)
			return (false); // Malloc error
		if (res == true)
			continue;
		if (!token_word(prompt, &i, tkn_list))
			return (false); // Malloc error
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
