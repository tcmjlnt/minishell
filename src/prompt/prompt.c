/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:51:31 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/14 17:17:00 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


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
		cmd_current = cmd_current->next;
	}
}

void	ft_prompt(t_shell *shell)
{
	char		*prompt;
	t_cmd		*cmd_list;



	while (1)
	{
		cmd_list = NULL;

		prompt = readline("minishell: ");
		if (!prompt)
		{
			error_exit("readline");
		}
		if (*prompt)
		{
			add_history(prompt);

			if (!parsing(prompt, &cmd_list, shell))
			{
				free(prompt);
				cmd_list = NULL;
				continue ;
			}
			// print_args(cmd_list);
			// printf("  fd_in  = %d\n", cmd_list->fd_in);
			// printf("  fd_out = %d\n", cmd_list->fd_out);

			exec_dispatcher(cmd_list, shell);
			// ceci est un commentaire pour le merge
			// printf("%d\n", x);
			// printf("Tu as tapé : %s\n", prompt); // a sup

			// print_args(cmd_list);

			free_tcmd(cmd_list);
			// cmd_list = NULL;

		}
		free(prompt);
	}
}
