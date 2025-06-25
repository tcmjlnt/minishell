/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 10:25:00 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/25 15:50:18 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_signals(void)
{
	g_sig = 0;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	signal_handler(int sig)
{
	(void) sig;
	g_sig = SIGINT;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	signal_handler_exec(int sig)
{
	if (sig == SIGINT)
	{
		g_sig = SIGINT;
		// write(1, "\n", 1);
	}
}

void	set_signals_interactive(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	set_signals_exec(void)
{
	signal(SIGINT, signal_handler_exec);
	signal(SIGQUIT, SIG_IGN);
}
void	signal_handler_heredoc(int sig)
{
	(void)sig;
	g_sig = SIGINT;
	write(1, "\n", 1);
	close(STDIN_FILENO);
} // force readline a return NULL et a sortir de la boucle
