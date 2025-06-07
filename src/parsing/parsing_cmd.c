/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 14:24:49 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/06 21:01:16 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"



// int	add_arg(t_token *temp, t_cmd *cmd_list, t_cmd *cmd)
// {
// 	int	j;

// 	j = 0;
// 	cmd = ft_lstnewcmd(temp, TOKEN_WORD, &j);
// 	if (!cmd)
// 		return (false);


// 	while (temp) // on parcourt la chaine jusquau dernier noeud
// 	{
// 		// si premier passage ou si pipe
// 		// creer un nouveau noeud
// 		// le rattacher a la liste

// 		if (temp->token_type == TOKEN_PIPE && temp->next) // si on tombe sur un PIPE on SKIP le token PIPE et on passe a la commande suivante. bien sur faut que la prochain noeud existe
// 		{
// 			cmd->args[j] = NULL; // (ou ft_strdup(""))?
// 			temp = temp->next;
// 			j = 0;
// 			cmd = ft_lstnewcmd(temp, TOKEN_PIPE, &j);
// 			if (!cmd)
// 				return (false);
// 			j++;
// 		}
// 		else // sinon remplir le tableau de string des arguments char **args
// 		{
// 			cmd->args[j] = ft_strdup(temp->token_value);
// 			j++;


// 		}
// 		ft_lstadd_back_cmd(&cmd_list, cmd);
// 		temp = temp->next;
// 	}
// 	return (true);

// }

int	parse_tokens(t_cmd **cmd_list_head, t_token **token_list_head) // passer la liste de token a la liste de cmd
{
	t_token *current_token;
	t_cmd	*current_cmd;
	int		j;

	j = 0;
	current_cmd = NULL;

	if (!token_list_head || !(*token_list_head)) // si ya pas de token_list nsm on se casse
		return (false);
	// if (!cmd_list)
	// 	return (printf("la\n"), false);
	current_token = *token_list_head;
	while (current_token && current_token->prev) // on remonte la liste ofc!!
		current_token = current_token->prev;

	while (current_token)
	{
		if (current_cmd == NULL)
		{
			current_cmd = ft_lstnewcmd();
			if (!current_cmd)
				return (false);
			ft_lstadd_back_cmd(cmd_list_head, current_cmd);
			j = 0;
		}
		if (current_token->token_type == TOKEN_PIPE)
		{
			if (j < 256)
				current_cmd->args[j] = NULL;
			else
				return (false); // too many args
			current_cmd = NULL;
			j = 0;
			if (!current_token->next || current_token->next->token_type == TOKEN_PIPE)
			{
				return (false);
				// This indicates a syntax error that should have been caught earlier.
				// printf("Syntax error near pipe handled by parse_tokens (should be earlier)\n");
				// return (false); // Or rely on earlier checks
			}
		}
		else if (current_token->token_type == TOKEN_WORD)
		{
			if (j >= 255)
			{
				printf("blabla\n");
				return (false);
			}
			current_cmd->args[j] = ft_strdup(current_token->token_value);
			if (!current_cmd->args[j]) // ft_strdup malloc failure
				return (false);
			if (j == 0)
			{
				current_cmd->cmd = ft_strdup(current_token->token_value);
				if (!current_cmd->cmd)
					return (false);
			}
			j++;
		}
		// Add handling for REDIRECTION tokens here.
		// They modify the current_cmd_node (e.g., infile, outfile) and consume the next token (filename).
		// Example:
		// else if (current_token->token_type == TOKEN_REDIRECT_OUT)
		// {
		//     if (!current_token->next || current_token->next->token_type != TOKEN_WORD)
		//         return (false); // Syntax error: > not followed by filename
		//     current_cmd_node->outfile = ft_strdup(current_token->next->token_value);
		//     // Set mode (truncate/append)
		//     current_token = current_token->next; // Consume filename token
		// }
		current_token = current_token->next;
	}

	if (current_cmd != NULL && j < 256)
		current_cmd->args[j] = NULL;
	else if (current_cmd != NULL && j >= 256)
		return (false);
	// if (!add_arg(temp, *cmd_list, cmd))
	// 	return (printf("la2\n"), false);

	return (true);
}
