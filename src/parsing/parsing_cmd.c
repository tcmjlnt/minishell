/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 14:24:49 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/17 16:36:34 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


int	parse_tokens(t_cmd **cmd_list_head, t_token **tkn_list) // passer la liste de token a la liste de cmd
{
	t_token *tkn_current;
	t_cmd	*cmd_current;
	t_redir *redir_list;
	int		j;

	j = 0;
	redir_list = NULL;
	cmd_current = NULL;

	if (!tkn_list || !(*tkn_list))  // si ya pas de tkn_list nsm on se casse
	{
		printf ("dabord la\n");
		return (false);
	}
	// if (!cmd_list) // cmd_list est set a nul puisquon le rempli dans cette fonction meme
	// 	return (printf("la\n"), false);
	tkn_current = *tkn_list;
	while (tkn_current && tkn_current->prev) // on remonte la liste ofc!!
		tkn_current = tkn_current->prev;

	while (tkn_current)
	{
		if (cmd_current == NULL)
		{
			cmd_current = ft_lstnewcmd();
			if (!cmd_current)
				return (false);
			ft_lstadd_back_cmd(cmd_list_head, cmd_current);
			j = 0;
			redir_list = NULL; // need to reset redir_list for new command
		}
		if (tkn_current->token_type == TOKEN_PIPE)
		{
			if (j < 256)
				cmd_current->args[j] = NULL;
			else
				return (false); // too many args
			cmd_current->redir = redir_list;
			cmd_current = NULL;
			j = 0;
			if (!tkn_current->next || tkn_current->next->token_type == TOKEN_PIPE)
			{
				return (false);
				// This indicates a syntax error that should have been caught earlier.
				// printf("Syntax error near pipe handled by parse_tokens (should be earlier)\n");
				// return (false); // Or rely on earlier checks
			}
		}
		else if(is_redir_operator(tkn_current->token_type))
		{
			if(tkn_current->next && tkn_current->next->token_type == TOKEN_WORD)
			{
				tkn_current=tkn_current->next;
				if(!fill_redir(&redir_list, tkn_current))
				{
					printf("pas marcher");
					return (false);
				}
			}
		}
		else if (tkn_current->token_type == TOKEN_WORD)
		{
			if (j >= 255)
			{
				printf("blabla faut pas abuser le nombre d'args stp\n");
				return (false);
			}
			cmd_current->args[j] = ft_strdup(tkn_current->token_value);
			if (!cmd_current->args[j]) // ft_strdup malloc failure
				return (false);
			if (j == 0)
			{
				cmd_current->cmd = ft_strdup(tkn_current->token_value);
				if (!cmd_current->cmd)
					return (false);
			}
			j++;
		}
		// Add handling for REDIRECTION tokens here.
		// They modify the cmd_current_node (e.g., infile, outfile) and consume the next token (filename).
		// Example:
		// else if (tkn_current->token_type == TOKEN_REDIRECT_OUT)
		// {
		//     if (!tkn_current->next || tkn_current->next->token_type != TOKEN_WORD)
		//         return (false); // Syntax error: > not followed by filename
		//     cmd_current_node->outfile = ft_strdup(tkn_current->next->token_value);
		//     // Set mode (truncate/append)
		//     tkn_current = tkn_current->next; // Consume filename token
		// }
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
	// if (!add_arg(temp, *cmd_list, cmd))
	// 	return (printf("la2\n"), false);

	return (true);
}
