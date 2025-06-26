/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:51:31 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/26 17:57:37 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


void	ft_prompt(t_shell *shell)
{
	char		*prompt;
	t_cmd		*cmd_list;

	init_signals();

	while (1)
	{
		gc_mem(GC_FREE_ALL, 0, NULL, GC_CMD);

		cmd_list = NULL;
		// init_signals();
		prompt = readline(MAGENTA "minishell:" RESET);
		if (g_sig == SIGINT)
		{
			shell->exit_status = 130;
			g_sig = 0;
		}
		if (!prompt)
		{
			int res = shell->exit_status;
			gc_mem(GC_FREE_ALL, 0, NULL, GC_CMD);
			rl_clear_history();
			ft_putstr_fd("exit\n", 2);
			exit(res);
			// error_exit("readline");
		}
/* 		else if (prompt[0] == '\0') // c'est cense gerer les ctrl + '\'
		{
			gc_mem(GC_FREE_ALL, 0, NULL, GC_CMD);
		} */
		if (*prompt)
		{
			add_history(prompt);

			if (!parsing(prompt, &cmd_list, shell))
			{
				shell->exit_status = 2;
				free(prompt);
				gc_mem(GC_FREE_ALL, 0 , NULL, GC_TKN);
				gc_mem(GC_FREE_ALL, 0, NULL, GC_CMD);
				cmd_list = NULL;
				continue ;
			}

			gc_mem(GC_FREE_ALL, 0, NULL, GC_TKN);
			// print_args(cmd_list);
			// printf("  fd_in  = %d\n", cmd_list->fd_in);
			// printf("  fd_out = %d\n", cmd_list->fd_out);

			set_signals_exec();

			exec_dispatcher(cmd_list, shell);

			if (g_sig == SIGINT)
			{
				shell->exit_status = 130;
				g_sig = 0;
			}

			set_signals_interactive();


			gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);

			// ceci est un commentaire pour le merge
			// printf("%d\n", x);
			// printf("Tu as tapé : %s\n", prompt); // a sup

			// print_args(cmd_list);

			// free_tcmd(cmd_list);
			// cmd_list = NULL;

		}
		// free(prompt);
	}
}
