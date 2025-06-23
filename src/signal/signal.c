/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 19:53:20 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/22 22:25:05 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_signals(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
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
