/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:51:31 by aumartin          #+#    #+#             */
/*   Updated: 2025/05/28 17:16:15 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


void	ft_prompt(t_shell *shell)
{
	char		*prompt;

	while (1)
	{
		prompt = readline("Je dis a minishell >");
		if (!prompt)
		{
			error_exit("readline");
		}
		if (*prompt)
		{
			add_history(prompt);

			if (!parsing(prompt, shell))
			{
				free(prompt);
				continue ;
			}
			// printf("%d\n", x);
			// printf("Tu as tapé : %s\n", prompt); // a sup
		}
		free(prompt);
	}
}
