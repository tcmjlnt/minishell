/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:51:31 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/09 22:56:52 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


void	ft_prompt(t_shell *shell)
{
	char		*prompt;
	t_cmd		*cmd_list;

	cmd_list = NULL;


	while (1)
	{
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

			exec_dispatcher(cmd_list, shell);
			// ceci est un commentaire pour le merge
			// printf("%d\n", x);
			// printf("Tu as tapé : %s\n", prompt); // a sup
		}
		free(prompt);
	}
}
