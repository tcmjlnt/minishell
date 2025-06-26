/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deprecated.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:00:22 by tjacquel          #+#    #+#             */
<<<<<<< HEAD:src/parsing/deprecated.c
/*   Updated: 2025/06/26 17:22:52 by aumartin         ###   ########.fr       */
=======
/*   Updated: 2025/06/26 18:22:30 by tjacquel         ###   ########.fr       */
>>>>>>> main:tests/deprecated.c
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// anciennes fonctions ou anciens bout de fonctions

<<<<<<< HEAD:src/parsing/deprecated.c
void	free_tcmd(t_cmd *cmd_list)
{
	t_cmd	*cmd_current;
	t_cmd	*next_cmd;
	t_redir	*current_redir;
	t_redir	*next_redir;
	int		j = 0;

	if (!cmd_list)
		return ;

	cmd_current = cmd_list;

	while(cmd_current)
	{
		next_cmd = cmd_current->next;
		if (cmd_current->cmd)
		{
			free(cmd_current->cmd);
			// cmd_current->cmd = NULL;
		}
		if (cmd_current->args)
		{
			j = 0;
			while (cmd_current->args[j])
			{
				free (cmd_current->args[j]);
				// cmd_current->args[j] = NULL;
				j++;
			}
			free(cmd_current->args);
			// cmd_current->args = NULL;
		}
		if (cmd_current->redir)
		{
			current_redir = cmd_current->redir;
			while (current_redir)
			{
				next_redir = current_redir->next;
				if (current_redir->file)
				{
					free(current_redir->file);
					// current_redir->file = NULL;
				}
				free(current_redir);
				current_redir = next_redir;
			}
			// cmd_current->redir = NULL;
		}

		free(cmd_current);
		cmd_current = next_cmd;
	}
}

=======
int	parse_tokens(t_cmd **cmd_list_head, t_token **tkn_list, t_shell *shell)
{
	t_token	*tkn_current;
	t_cmd	*cmd_current;
	t_redir	*redir_list;
	int		j;

	j = 0;
	redir_list = NULL;
	cmd_current = NULL;
	if (!tkn_list || !(*tkn_list))
		return (false);
	tkn_current = *tkn_list;
	while (tkn_current)
	{
		if (cmd_current == NULL)
		{
			cmd_current = ft_lstnewcmd();
			if (!cmd_current)
				return (false);
			ft_lstadd_back_cmd(cmd_list_head, cmd_current);
			j = 0;
			redir_list = NULL;
		}
		if (tkn_current->token_type == TKN_PIPE)
		{
			if (j < 256)
				cmd_current->args[j] = NULL;
			else
				return (false);
			cmd_current->redir = redir_list;
			cmd_current = NULL;
			j = 0;
			if (!tkn_current->next || tkn_current->next->token_type == TKN_PIPE)
			{
				shell->exit_status = 0;
				return (false);
			}
		}
		else if (is_redir_operator(tkn_current->token_type))
		{
			if (tkn_current->next && tkn_current->next->token_type == TKN_WORD)
			{
				tkn_current = tkn_current->next;
				if (!fill_redir(&redir_list, tkn_current))
					return (false);
			}
		}
		else if (tkn_current->token_type == TKN_WORD)
		{
			if (j >= 255)
			{
				printf("blabla faut pas abuser le nombre d'args stp\n");
				return (false);
			}
			cmd_current->args[j] = gc_strdup(tkn_current->token_value, GC_CMD);
			if (!cmd_current->args[j])
			{
				return (false);
			}
			if (j == 0)
			{
				cmd_current->cmd = gc_strdup(tkn_current->token_value, GC_CMD);
				if (!cmd_current->cmd)
				{
					return (false);
				}
				cmd_current->is_builtin = is_builtin(shell, cmd_current->cmd);
			}
			j++;
		}
		tkn_current = tkn_current->next;
	}
	if (cmd_current != NULL)
	{
		cmd_current->redir = redir_list;
		if (j < 256)
			cmd_current->args[j] = NULL;
		else
			return (false);
	}
	return (true);
}

int	pipes_check(char *prompt)
{
	int	i;

	i = 0;
	while (prompt[i] && is_blank(prompt[i]))
		i++;
	if (prompt[i] == '|' && !is_inside_quotes(prompt, i))
	{
		ft_putstr_fd(PIPE_SYNT_ERR, 2);
		return (false);
	}
	i = 0;
	while (prompt[i])
	{
		if (prompt[i] == '|' && !is_inside_quotes(prompt, i))
		{
			if (prompt[i + 1] && prompt[i + 1] == '|')
			{
				ft_putstr_fd(PIPE_SYNT_ERR, 2);
				return (false);
			}
			i++;
			while (prompt[i] && is_blank(prompt[i]))
				i++;
			if (prompt[i] == '|')
			{
				ft_putstr_fd(PIPE_SYNT_ERR, 2);
				return (false);
			}
			if (!prompt[i])
			{
				ft_putstr_fd(PIPE_SYNT_ERR, 2);
				return (false);
			}
			continue ;
		}
		i++;
	}
	return (true);
}

int	tokenize_prompt(char *prompt, t_token **tkn_list) // en cours de refacto
{
	t_token	*token;
	int		i;
	int		start;

	i = 0;
	start = 0;
	while (prompt[i])
	{
		while (prompt[i] && is_blank(prompt[i]))
			i++;
		if (!prompt[i])
			break ;
		if(prompt[i] == '|')
		{
			start = i;
			token = ft_lstnewtoken(prompt + start, 1, TKN_PIPE);
			if (!token)
				return (false);
			i++;
		}
		else if(prompt[i] == '<' && prompt[i + 1] && prompt[i + 1] == '<')
		{
			start = i;
			token = ft_lstnewtoken(prompt + start, 2, TKN_HEREDOC);
			if (!token)
				return (false);
			i += 2;
		}
		// else if (!handle_append_token(prompt, &i, token, tkn_list))
		// 	return (false);
		else if(prompt[i] == '>' && prompt[i + 1] && prompt[i + 1] == '>')
		{
			start = i;
			token = ft_lstnewtoken(prompt + start, 2, TKN_APPEND);
			if (!token)
				return (false);
			i += 2;
		}
		else if(prompt[i] == '>')
		{
			start = i;
			token = ft_lstnewtoken(prompt + start, 1, TKN_OUT);
			if (!token)
				return (false);
			i++;
		}
		else if(prompt[i] == '<')
		{
			start = i;
			token = ft_lstnewtoken(prompt + start, 1, TKN_IN);
			if (!token)
				return (false);
			i++;
		}
		else
		{
			token = token_wordtpye(prompt, &i);
			if (!token)
				return (false);
		}
		ft_lstadd_back_token(tkn_list, token);
	}
}

int	dquotes_scnd_segmentation(char *substr, t_xpnd *xpnd_quotes_curr,
								t_xpnd **xpnd_list, t_token *tkn_curr) // before refacto
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

int	noquotes_scnd_segmentation(char *substr, t_xpnd *xpnd_quotes_curr, t_xpnd **xpnd_list, t_token *tkn_curr) // avant refacto
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

int	quotes_first_segmentation(char *tkn_raw, t_xpnd **xpnd_list) // avant refacto
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
			if (!create_unquoted_segment(tkn_raw, start, i, xpnd_list))
				return (false);
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
	if (!create_unquoted_segment(tkn_raw, start, i, xpnd_list))
		return (false);
	return (true);
}

>>>>>>> main:tests/deprecated.c
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
		free (temp);
		if (!res)
			return (false);
		temp = ft_strdup(res);
		if (!temp)
			return (false);
		xpnd_curr = xpnd_curr->next;
	}
	free (temp);
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
	free (res);

	return (true);
}


int	join_xpnd_gem2(t_xpnd **xpnd_list, t_token **tkn_xpnd_list, t_token *tkn_current) // gemini 2
{
	t_xpnd	*xpnd_curr;
	t_token	*tkn_xpnd_curr;
	char	*result;
	char	*temp;

	if (!xpnd_list || !(*xpnd_list))
		return (true); // No nodes to process, do nothing.
	result = NULL; // Initialize result to NULL.

	xpnd_curr = *xpnd_list;
	while (xpnd_curr)
	{
		// This is the crucial logic to skip unquoted empty variables.
		if (xpnd_curr->xpnd_check && !xpnd_curr->in_single && !xpnd_curr->in_double
			&& xpnd_curr->str_to_join && xpnd_curr->str_to_join[0] == '\0')
		{
			xpnd_curr = xpnd_curr->next;
			continue;
		}
		if (result == NULL) // First non-skipped node.
			result = ft_strdup(xpnd_curr->str_to_join);
		else // Subsequent nodes.
		{
			temp = ft_strjoin(result, xpnd_curr->str_to_join);
			free(result);
			result = temp;
		}
		if (!result)
			return (false); // Malloc failed.
		xpnd_curr = xpnd_curr->next;
	}

	// If result is still NULL, it means all nodes were skipped (e.g., only $DONT).
	// In this case, we create no token, effectively removing the argument.
	if (result == NULL)
		return (true);

	// Otherwise, create a token with the resulting string.
	tkn_xpnd_curr = ft_lstnewtoken_xpnd();
	if (!tkn_xpnd_curr)
	{
		free(result);
		return (false);
	}

	tkn_xpnd_curr->token_raw = ft_strdup(tkn_current->token_raw);
	tkn_xpnd_curr->token_type = tkn_current->token_type;
	tkn_xpnd_curr->token_value = result; // Assign final string.
	if (!tkn_xpnd_curr->token_raw)
	{
		free(tkn_xpnd_curr->token_value);
		free(tkn_xpnd_curr);
		return (false);
	}
	ft_lstadd_back_token(tkn_xpnd_list, tkn_xpnd_curr);
	return (true);
}


int	lexer(char *prompt, t_token *token) // premiere iteration du lexer jai change de voie apres
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
			token = ft_lstnew_token(value, TKN_WORD, 2);
		}
		if (is_blank(prompt[i]))
			i++;
		i++;
	}
	// token->value = ft_strdup("bonjour");
	return (1);

}

int first_syntax_check(char *prompt) // javais inclus le '&' mais retire ensuite sinon la focntion actuelle c'est la meme
{

	if (!closed_quotes(prompt))
	{
		printf("Unclosed quotes\n");
		return (false);
	}

	if (!pipes_check(prompt))
		return (false);
	// i = 0;
	// while(prompt[i])
	// {
	// 	if(prompt[i + 1] && (prompt[i] == '&' && prompt[i + 1] == '&'))
	// 	{
	// 		printf("Syntax error %c\n", prompt[i]);
	// 		return false;
	// 	}
	// 	i++;
	// }
	return (true);
}

void	old_first_parse(char *prompt) // premiere version degrossie quick 'n' dirty parsing ft. Eric
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return;
	cmd->next = NULL;
	cmd->prev = NULL;
	cmd->args = malloc(sizeof(char*) * 256);
	char **first_split = ft_split(prompt, ' ');
	int i = 0;
	int j = 0;
	while (first_split[i])
	{
		if (ft_strncmp(first_split[i], "|", 1) == 0)  // Compare 1 character, check if equal
		{
			// Null-terminate current command's args
			cmd->args[j] = NULL;

			// Create new command node
			cmd->next = malloc(sizeof(t_cmd));
			if (!cmd->next)
				return ;
			cmd->next->prev = cmd;
			cmd->next->next = NULL;
			cmd->next->args = malloc(sizeof(char*) * (MAX_ARGS + 1));

			// Move to new command
			cmd = cmd->next;
			j = 0;  // Reset arg counter
			i++;
			continue;
		}
			cmd->args[j] = ft_strdup(first_split[i]);

			i++;
			j++;
			if (j >= MAX_ARGS)
			{
				printf("Error: too many arguments (max 255)\n");
				return ;
			}

	}
	cmd->args[j] = NULL;
	print_args(cmd);

}


int	parsing(char *prompt, t_shell *shell) // gros bout de code avant nettoyage qui fonctionne egalement avec old_first_parse() si decommente
{
	(void)	shell;
	// t_cmd	*cmd;
	t_token	*token;
	t_token *tkn_list;

	tkn_list = NULL;
	// cmd = malloc(sizeof(t_cmd));
	// if (!cmd)
	// 	return(false);
	// cmd->next = NULL;
	// cmd->prev = NULL;
	// cmd->args = malloc(sizeof(char*) * 256);
	// if(!cmd->args)
	// 	return(false);

	if (!first_syntax_check(prompt))
	{
		return (false);
	}
	// char *content = ft_strdup("bonjour");
	// token = ft_lstnew_token(content, 2, 3);
	// old_first_parse(prompt);
	int i = 0;
	// int j = 0;
	int start = 0;
	// char *arg = NULL;
	while (prompt[i])
	{
		while (prompt[i] && is_blank(prompt[i])) // skip blanks (' ' || '\t')
			i++;
		if (!prompt[i])
			break ;
		if (prompt[i] == '\"') // D_QUOTE word
		{
			while (prompt[i] == '\"')
				i++;
			start = i;

			while (prompt[i] && prompt[i] != '\"')
				i++;
			token = ft_lstnewtoken(prompt + start, i - start, TOKEN_D_QUOTES);
			if (!token)
				return (false);
			while (prompt[i] == '\"')
				i++;
		}
		else if (prompt[i] == '\'') // S_QUOTE word
		{
			while (prompt[i] == '\'')
				i++;
			start = i;

			while (prompt[i] && prompt[i] != '\'')
				i++;
			token = ft_lstnewtoken(prompt + start, i - start, TOKEN_S_QUOTES);
			if (!token)
				return (false);
			while (prompt[i] == '\'')
				i++;
		}
		else if(prompt[i] == '|') // PIPE
		{
			start = i;
			token = ft_lstnewtoken(prompt + start, 1, TKN_PIPE);
			if (!token)
				return (false);
			i++;
		}
		else if(prompt[i] == '<' && prompt[i + 1] && prompt[i + 1] == '<') // RED_HEREDOC
		{
			start = i;
			token = ft_lstnewtoken(prompt + start, 2, TKN_HEREDOC);
			if (!token)
				return (false);
			i += 2;
		}
		else if(prompt[i] == '>' && prompt[i + 1] && prompt[i + 1] == '>') // RED_APPEND
		{
			start = i;
			token = ft_lstnewtoken(prompt + start, 2, TKN_APPEND);
			if (!token)
				return (false);
			i += 2;
		}
		else if(prompt[i] == '>') // RED_OUT
		{
			start = i;
			token = ft_lstnewtoken(prompt + start, 1, TKN_OUT);
			if (!token)
				return (false);
			i++;
		}
		else if(prompt[i] == '<') // RED_IN
		{
			start = i;
			token = ft_lstnewtoken(prompt + start, 1, TKN_IN);
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
			// token = ft_lstnewtoken(prompt + start, i - start, TKN_WORD);
			// if (!token)
			// 	return (false);
		}
		ft_lstadd_back_token(&tkn_list, token);

		// cmd->args[j] = arg;
		// printf("arg[%d]: %s\n", j, arg);

		// j++;

		// else if(prompt[i] = '\'')

	}

	if (!check_token(tkn_list))
	{
		return (false);
	}


	//printf("%s\n", prompt);
	// lexer(prompt, token);
	print_token(tkn_list);
	return (1);
}
