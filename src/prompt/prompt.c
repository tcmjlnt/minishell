/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:51:31 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/27 16:42:25 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


void	ft_prompt(t_shell *shell)
{
	char		*prompt;
	t_cmd		*cmd_list;
	int			res;

	init_signals();

	while (1)
	{
		cmd_list = NULL;
		prompt = readline(MAGENTA "minishell:" RESET);
		if (g_sig == SIGINT)
		{
			shell->exit_status = 130;
			g_sig = 0;
		}
		if (!prompt)
		{
			res = shell->exit_status;
			gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);
			rl_clear_history();
			ft_putstr_fd("exit\n", 2);
			exit(res);
		}
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
			set_signals_exec();
			exec_dispatcher(cmd_list, shell);
			if (g_sig == SIGINT)
			{
				shell->exit_status = 130;
				g_sig = 0;
			}
			set_signals_interactive();
			gc_mem(GC_FREE_ALL, 0, NULL, GC_CMD);
		}
		// free(prompt);
	}
}
