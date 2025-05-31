/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:17:55 by tjacquel          #+#    #+#             */
/*   Updated: 2025/05/31 20:54:08 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_separator(char c)
{
	if (c == '\'' || c == '\"' || c == '|' || c == '<' || c == '>')
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

// char	which_quote(char c)
// {
// 	if (c == "\'")
// 		return ("\'");
// 	else if (c == "\"")
// 		return ("\"");
// }

int first_syntax_check(char *prompt)
{

	int i = 0;
	if (!closed_quotes(prompt))
	{
		printf("Unclosed quotes\n");
		return (false);
	}
	// while(prompt[i])
	// {
	// 	if (!closed_quotes(prompt))
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

void	print_args(t_cmd *cmd)
{
	int i = 0;
	int j = 0;

	while(cmd->prev)
	{
		cmd = cmd->prev;
	}
	while (cmd)
	{
		printf("Command %d:\n", i);
		if (cmd->args)
		{
			j = 0;
			while(cmd->args[j])
			{
				printf("arg[%d]: %s\n", j, cmd->args[j]);
				j++;
			}
		}
		i++;
		cmd = cmd->next;
	}
}

// void	first_lexing

void	old_first_parse(char *prompt)
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



int	parsing(char *prompt, t_shell *shell)
{
	(void)	shell;
	// t_cmd	*cmd;
	t_token	*token;
	t_token *token_list;

	token_list = NULL;
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
			// arg = ft_strndup(prompt + start, i - start);
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
			// arg = ft_strndup(prompt + start, i - start);
			token = ft_lstnewtoken(prompt + start, i - start, TOKEN_S_QUOTES);
			if (!token)
				return (false);
			while (prompt[i] == '\'')
				i++;
			// token->value = start -> i;
		}
		else if(prompt[i] == '|') // ca va pas marcher comme ca, besoin de traiter les listes chainees
		{
			start = i;
			token = ft_lstnewtoken(prompt + start, 1, TOKEN_PIPE);
			if (!token)
				return (false);
			// arg = ft_strndup(prompt + start, 1);
			i++;
		}
		else // standard word
		{
			start = i;
			while (prompt[i] && !is_blank(prompt[i]) && !is_separator(prompt[i]))
				i++;
			// arg = ft_strndup(prompt + start, i - start);
			token = ft_lstnewtoken(prompt + start, i - start, TOKEN_WORD);
			if (!token)
				return (false);
		}
		ft_lstadd_back_token(&token_list, token);

		// cmd->args[j] = arg;
		// printf("arg[%d]: %s\n", j, arg);

		// j++;

		// else if(prompt[i] = '\'')

	}

	//printf("%s\n", prompt);
	// lexer(prompt, token);
	print_token(token_list);
	return (1);
}


// int
// {
// 	split le prompt sur les whitespaces--> char **
// 	itere sur le i de splitted prompt[i]
// 	l'idee c'est de code le argv de

// }