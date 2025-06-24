/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:53:20 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/24 15:48:58 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_signals(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	signal_handler(int sig)
{
	(void) sig;
	g_sig = 1;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	signal_handler_exec(int sig)
{
	if (sig == SIGINT)
	{
		g_sig = 1;
		write(1, "\n", 1);
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

// 🔁 Rouage complet (vue schématique)
/* MAIN LOOP (parent)
│
├─ Setup signaux parent (SIGINT = propre, SIGQUIT = ignorer)
│
├─ readline()
│   └─ si CTRL+C → effacer ligne + prompt propre
│
├─ si commande normale
│   ├─ fork()
│   │   ├─ parent : waitpid()
│   │   └─ enfant : setup signaux défaut → execve(cmd)
│
└─ si heredoc
    ├─ fork() ou logique spéciale
    ├─ setup signaux heredoc
    └─ si SIGINT → interrompre lecture, fermer stdin
 */
