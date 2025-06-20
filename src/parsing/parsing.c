/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:17:55 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/20 19:48:31 by tjacquel         ###   ########.fr       */
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

static int	is_blank(int c)
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
		printf("	redir_file[%d]: `%s`	;	redir_type[%d]: `%d`\n", i, redir_current->file, i, redir_current->type);
		i++;
		redir_current = redir_current->next;
	}
}

void	print_args(t_cmd *cmd)
{
	int i = 0;
	int j = 0;
	t_cmd *temp = cmd;

	while(temp && temp->prev)
	{
		temp = temp->prev;
	}
	while (temp)
	{
		printf("-----------------\nCommand %d: `%s` ; is_builtin=%d ; cmd_type=%d\n", i, temp->cmd, temp->is_builtin, temp->cmd_type);
		if (temp->args)
		{
			j = 0;
			while(temp->args[j])
			{
				printf("arg[%d]: `%s`\n", j, temp->args[j]);
				j++;
			}
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
	temp = ft_lstnewtoken(prompt + start, (*i) - start, TOKEN_WORD);
	if (!temp)
		return (NULL);

	return (temp);

}


int	parsing(char *prompt, t_cmd **cmd_list, t_shell *shell)
{
	(void)	shell;
	t_token	*token;
	t_token	*tkn_list;
	t_token	*tkn_xpnd_list;
	// t_cmd	*cmd_list;

	tkn_list = NULL;
	tkn_xpnd_list = NULL;
	// cmd_list = NULL;


	if (!first_syntax_check(prompt, shell))
	{
		return (false);
	}

	int i = 0;
	int start = 0;
	while (prompt[i])
	{
		while (prompt[i] && is_blank(prompt[i])) // skip blanks (' ' || '\t')
			i++;
		if (!prompt[i])
			break ;
		// if (prompt[i] == '\"') // D_QUOTE word
		// {
		// 	while (prompt[i] == '\"')
		// 		i++;
		// 	start = i;

		// 	while (prompt[i] && prompt[i] != '\"')
		// 		i++;
		// 	token = ft_lstnewtoken(prompt + start, i - start, TOKEN_D_QUOTES);
		// 	if (!token)
		// 		return (false);
		// 	while (prompt[i] == '\"')
		// 		i++;
		// }
		// else if (prompt[i] == '\'') // S_QUOTE word
		// {
		// 	while (prompt[i] == '\'')
		// 		i++;
		// 	start = i;

		// 	while (prompt[i] && prompt[i] != '\'')
		// 		i++;
		// 	token = ft_lstnewtoken(prompt + start, i - start, TOKEN_S_QUOTES);
		// 	if (!token)
		// 		return (false);
		// 	while (prompt[i] == '\'')
		// 		i++;
		// }
		if(prompt[i] == '|') // PIPE
		{
			start = i;
			token = ft_lstnewtoken(prompt + start, 1, TOKEN_PIPE);
			if (!token)
				return (false);
			i++;
		}
		else if(prompt[i] == '<' && prompt[i + 1] && prompt[i + 1] == '<') // RED_HEREDOC
		{
			start = i;
			token = ft_lstnewtoken(prompt + start, 2, TOKEN_REDIRECT_HEREDOC);
			if (!token)
				return (false);
			i += 2;
		}
		else if(prompt[i] == '>' && prompt[i + 1] && prompt[i + 1] == '>') // RED_APPEND
		{
			start = i;
			token = ft_lstnewtoken(prompt + start, 2, TOKEN_REDIRECT_APPEND);
			if (!token)
				return (false);
			i += 2;
		}
		else if(prompt[i] == '>') // RED_OUT
		{
			start = i;
			token = ft_lstnewtoken(prompt + start, 1, TOKEN_REDIRECT_OUT);
			if (!token)
				return (false);
			i++;
		}
		else if(prompt[i] == '<') // RED_IN
		{
			start = i;
			token = ft_lstnewtoken(prompt + start, 1, TOKEN_REDIRECT_IN);
			if (!token)
				return (false);
			i++;
		}
		else // standard word
		{
			token = token_wordtpye(prompt, &i);
			if (!token)
				return (false);
			/* option 1: garder 3 tokens pour hell"o   w"orld, concatener ensuite par exemple
							variable dans le t_token du type char_before/after_quote
			option 2: reussir a avoir 1 seul token, garder la valeur raw du token avec les quotes */

			// start = i;
			// while (prompt[i] && !is_blank(prompt[i]) && !is_operator(prompt[i]))
			// 	i++;
			// token = ft_lstnewtoken(prompt + start, i - start, TOKEN_WORD);
			// if (!token)
			// 	return (false);
		}
		ft_lstadd_back_token(&tkn_list, token);





	}

	if (!check_token(tkn_list))
	{
		return (false);
	}
	// print_token(tkn_list);

	handle_expansion(&tkn_list, &tkn_xpnd_list, shell); // eet si on faisait l'expand apres la token avant le parsing

	// printf("-------------------- TOKENS AFTER EXPANSION -----------------\n");
	// print_token(tkn_xpnd_list);


	if (!parse_tokens(cmd_list, &tkn_xpnd_list, shell))
	{
		printf("ici\n");
		return (false);
	}

	// print_args(*cmd_list);
	// handle_expansion(&cmd_list, &tkn_list, shell);
	// expand
	// redirections --> penser a creer un liste chainee pour les redirections
	// parsing to exec

	return (1);
}

