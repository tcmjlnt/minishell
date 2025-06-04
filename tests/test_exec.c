/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:58:59 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/04 16:44:26 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

void	test_exec_cmd(t_shell *shell, t_cmd *cmd)
{
	t_cmd	*cmd1;
	t_cmd	*cmd2;
	t_cmd	*cmd3;
	int		fd_out;

	char	*args1[] = {"ls", "-l", NULL};
	char	*args2[] = {"pwd", NULL};
	char	*args3[] = {"echo", "-n", "hello AURORE", NULL};

	// cree les commandes avec create_cmd
	cmd1 = create_cmd("ls", args1, STDIN_FILENO, STDOUT_FILENO, false);
	cmd2 = create_cmd("pwd", args2, STDIN_FILENO, STDOUT_FILENO, false);
	cmd3 = create_cmd("echo", args3, STDIN_FILENO, STDOUT_FILENO, false);

	// chaine les cmds
	cmd1->next = cmd2;
	cmd2->prev = cmd1;
	cmd2->next = cmd3;
	cmd3->prev = cmd2;
	cmd3->next = NULL;

	// redirection
	fd_out = open("test_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	cmd1->fd_out = fd_out;
	cmd2->fd_out = fd_out;
	// cmd3->fd_out = fd_out;

	// exec les commandes
	exec_cmds(cmd1, shell->env);

	close(fd_out);

	// free
	free(cmd1);
	free(cmd2);

}
