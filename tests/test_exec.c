/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:58:59 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/10 09:53:44 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

/* void	test_exec_cmd(t_shell *shell, t_cmd *cmd)
{
	t_cmd	*cmd1;
	t_cmd	*cmd2;
	t_cmd	*cmd3;
	t_cmd	*cmd4;
	int		fd_out;

	(void) cmd;
	char	*args1[] = {"ls", "-l", NULL};
	char	*args2[] = {"pwd", NULL};
	char	*args3[] = {"echo", "-n", "hello AURORE", NULL};
	char	*args4[] = {"cat", "test_output.txt", NULL};

	// cree les commandes avec create_cmd
	cmd1 = create_cmd("ls", args1, STDIN_FILENO, STDOUT_FILENO, shell);
	cmd2 = create_cmd("pwd", args2, STDIN_FILENO, STDOUT_FILENO, shell);
	cmd3 = create_cmd("echo", args3, STDIN_FILENO, STDOUT_FILENO, shell);
	cmd4 = create_cmd("cat", args4, STDIN_FILENO, STDOUT_FILENO, shell);

	// chaine les cmds
	cmd1->next = cmd2;
	cmd2->prev = cmd1;
	cmd2->next = cmd3;
	cmd3->prev = cmd2;
	cmd3->next = cmd4;
	cmd4->prev = cmd3;
	cmd4->next = NULL;

	// redirection
	fd_out = open("test_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	cmd1->fd_out = fd_out;
	cmd2->fd_out = fd_out;
	//cmd3->fd_out = fd_out;

	// exec les commandes
	exec_cmds(cmd1, shell->env);

	close(fd_out);

	// free
	free(cmd1);
	free(cmd2);
	free(cmd3);
	free(cmd4);
} */

// Test simple : ls | wc -l
void	test_pipe_ls_wc(t_shell *shell)
{
	t_cmd *cmd1, *cmd2;
	char *args1[] = {"ls", NULL};
	char *args2[] = {"wc", "-l", NULL};

	cmd1 = create_cmd("ls", args1, STDIN_FILENO, STDOUT_FILENO, shell);
	cmd2 = create_cmd("wc", args2, STDIN_FILENO, STDOUT_FILENO, shell);

	cmd1->next = cmd2;
	cmd2->prev = cmd1;

	printf("\nTest: ls | wc -l\n");
	exec_cmds(cmd1, shell);

	free(cmd1);
	free(cmd2);
}

// Test : pwd | echo coucou | ls
void	test_pipe_pwd_echo_ls(t_shell *shell)
{
	t_cmd *cmd1, *cmd2, *cmd3;
	char *args1[] = {"pwd", NULL};
	char *args2[] = {"echo", "coucou", NULL};
	char *args3[] = {"ls", NULL};

	cmd1 = create_cmd("pwd", args1, STDIN_FILENO, STDOUT_FILENO, shell);
	cmd2 = create_cmd("echo", args2, STDIN_FILENO, STDOUT_FILENO, shell);
	cmd3 = create_cmd("ls", args3, STDIN_FILENO, STDOUT_FILENO, shell);

	cmd1->next = cmd2;
	cmd2->prev = cmd1;
	cmd2->next = cmd3;
	cmd3->prev = cmd2;

	printf("\nTest: pwd | echo coucou | ls\n");
	exec_cmds(cmd1, shell);

	free(cmd1);
	free(cmd2);
	free(cmd3);
}

// Test simple avec builtin seul
void	test_pipe_single_builtin(t_shell *shell)
{
	t_cmd *cmd1;
	char *args1[] = {"pwd", NULL};

	cmd1 = create_cmd("pwd", args1, STDIN_FILENO, STDOUT_FILENO, shell);

	printf("\nTest: pwd (seul builtin dans pipeline)\n");
	exec_cmds(cmd1, shell);

	free(cmd1);
}

// Test : ls | unknowncmd
void	test_pipe_unknown_cmd(t_shell *shell)
{
	t_cmd *cmd1, *cmd2;
	char *args1[] = {"ls", NULL};
	char *args2[] = {"unknowncmd", NULL};

	cmd1 = create_cmd("ls", args1, STDIN_FILENO, STDOUT_FILENO, shell);
	cmd2 = create_cmd("unknowncmd", args2, STDIN_FILENO, STDOUT_FILENO, shell);

	cmd1->next = cmd2;
	cmd2->prev = cmd1;

	printf("\nTest: ls | unknowncmd (doit afficher une erreur)\n");
	exec_cmds(cmd1, shell);

	free(cmd1);
	free(cmd2);
}

// Fonction principale qui lance tous les tests
void	run_exec_pipes_tests(t_shell *shell)
{
	test_pipe_ls_wc(shell);
	test_pipe_pwd_echo_ls(shell);
	test_pipe_unknown_cmd(shell);
	test_pipe_single_builtin(shell);
	gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);
}
