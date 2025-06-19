/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deprecated.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:00:22 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/10 18:09:32 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// anciennes fonctions ou anciens bout de fonctions



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
			token = ft_lstnew_token(value, TOKEN_WORD, 2);
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