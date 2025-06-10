/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/08 11:27:46 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/10 13:58:16 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

// Test 1 : simple commande externe ls
void test_simple_ls(t_shell *shell)
{
	printf("\n[Test simple ls]\n");
	printf("Attendu : affichage des fichiers du répertoire courant.\n");

	char *args[] = {"ls", NULL};
	t_cmd *cmd = create_cmd("ls", args, STDIN_FILENO, STDOUT_FILENO, shell);

	// debug_cmd(cmd);
	exec_dispatcher(cmd, shell);
	free(cmd);
}

// Test 2 : single builtin pwd
void test_single_builtin_pwd(t_shell *shell)
{
	printf("\n[Test single builtin pwd]\n");
	printf("Attendu : affichage du chemin courant.\n");

	char *args[] = {"pwd", NULL};
	t_cmd *cmd = create_cmd("pwd", args, STDIN_FILENO, STDOUT_FILENO, shell);

	// debug_cmd(cmd);
	exec_dispatcher(cmd, shell);
	free(cmd);
}

// Test 3 : pipeline ls | wc -l
void test_pipeline_ls_wc(t_shell *shell)
{
	printf("\n[Test pipeline ls | wc -l]\n");
	printf("Attendu : nombre de fichiers/dossiers du répertoire courant.\n");

	char *args1[] = {"ls", NULL};
	char *args2[] = {"wc", "-l", NULL};

	t_cmd *cmd1 = create_cmd("ls", args1, STDIN_FILENO, STDOUT_FILENO, shell);
	t_cmd *cmd2 = create_cmd("wc", args2, STDIN_FILENO, STDOUT_FILENO, shell);
	cmd1->next = cmd2;
	cmd2->prev = cmd1;

	// debug_cmd_list(cmd1);
	exec_dispatcher(cmd1, shell);

	free(cmd1);
	free(cmd2);
}

// Test 4 : pipeline ls | grep .c | wc -l
void test_pipeline_multi(t_shell *shell)
{
	printf("\n[Test pipeline multi ls | grep .c | wc -l]\n");
	printf("Attendu : nombre de fichiers .c dans le répertoire courant.\n");

	char *args1[] = {"ls", NULL};
	char *args2[] = {"grep", ".c", NULL};
	char *args3[] = {"wc", "-l", NULL};

	t_cmd *cmd1 = create_cmd("ls", args1, STDIN_FILENO, STDOUT_FILENO, shell);
	t_cmd *cmd2 = create_cmd("grep", args2, STDIN_FILENO, STDOUT_FILENO, shell);
	t_cmd *cmd3 = create_cmd("wc", args3, STDIN_FILENO, STDOUT_FILENO, shell);
	cmd1->next = cmd2; cmd2->prev = cmd1;
	cmd2->next = cmd3; cmd3->prev = cmd2;

	// debug_cmd_list(cmd1);
	exec_dispatcher(cmd1, shell);

	free(cmd1);
	free(cmd2);
	free(cmd3);
}

// Test 5 : unknown command
void test_unknown_cmd(t_shell *shell)
{
	printf("\n[Test unknown command]\n");
	printf("Attendu : minishell: foobar: command not found\n");

	char *args[] = {"foobar", NULL};
	t_cmd *cmd = create_cmd("foobar", args, STDIN_FILENO, STDOUT_FILENO, shell);

	// debug_cmd(cmd);
	exec_dispatcher(cmd, shell);
	free(cmd);
}

// Test 6 : pipeline avec builtin suivi d’externe
void test_pipeline_builtin_externe(t_shell *shell)
{
	printf("\n[Test pipeline pwd | wc -l]\n");
	printf("Attendu : imprime 1 (car pwd génère 1 ligne)\n");

	char *args1[] = {"pwd", NULL};
	char *args2[] = {"wc", "-l", NULL};

	t_cmd *cmd1 = create_cmd("pwd", args1, STDIN_FILENO, STDOUT_FILENO, shell);
	t_cmd *cmd2 = create_cmd("wc", args2, STDIN_FILENO, STDOUT_FILENO, shell);
	cmd1->next = cmd2; cmd2->prev = cmd1;

	exec_dispatcher(cmd1, shell);

	free(cmd1);
	free(cmd2);
}

// Test 7 : pipeline avec plusieurs builtins
void test_pipeline_multi_builtins(t_shell *shell)
{
	printf("\n[Test pipeline pwd | echo test | wc -l]\n");
	printf("Attendu : imprime 1 (echo produit une ligne)\n");

	char *args1[] = {"pwd", NULL};
	char *args2[] = {"echo", "test", NULL};
	char *args3[] = {"wc", "-l", NULL};

	t_cmd *cmd1 = create_cmd("pwd", args1, STDIN_FILENO, STDOUT_FILENO, shell);
	t_cmd *cmd2 = create_cmd("echo", args2, STDIN_FILENO, STDOUT_FILENO, shell);
	t_cmd *cmd3 = create_cmd("wc", args3, STDIN_FILENO, STDOUT_FILENO, shell);

	cmd1->next = cmd2; cmd2->prev = cmd1;
	cmd2->next = cmd3; cmd3->prev = cmd2;

	exec_dispatcher(cmd1, shell);

	free(cmd1);
	free(cmd2);
	free(cmd3);
}

// Test 8 : pipeline avec unknown command au milieu
void test_pipeline_with_unknown_cmd(t_shell *shell)
{
	printf("\n[Test pipeline avec unknown cmd]\n");
	printf("Attendu : ls fonctionne, puis 'minishell: foobar: command not found', puis wc récupère une entrée vide\n");

	char *args1[] = {"ls", NULL};
	char *args2[] = {"foobar", NULL};
	char *args3[] = {"wc", "-l", NULL};

	t_cmd *cmd1 = create_cmd("ls", args1, STDIN_FILENO, STDOUT_FILENO, shell);
	t_cmd *cmd2 = create_cmd("foobar", args2, STDIN_FILENO, STDOUT_FILENO, shell);
	t_cmd *cmd3 = create_cmd("wc", args3, STDIN_FILENO, STDOUT_FILENO, shell);

	cmd1->next = cmd2; cmd2->prev = cmd1;
	cmd2->next = cmd3; cmd3->prev = cmd2;

	exec_dispatcher(cmd1, shell);

	free(cmd1);
	free(cmd2);
	free(cmd3);
}

// Test 9 : wc sans input (lecture depuis stdin)
void test_wc_without_input(t_shell *shell)
{
	printf("\n[Test wc -l sans input]\n");
	printf("Attendu : il attend l'entrée depuis le terminal (boucle stdin bloquante)\n");

	char *args[] = {"wc", "-l", NULL};
	t_cmd *cmd = create_cmd("wc", args, STDIN_FILENO, STDOUT_FILENO, shell);

	exec_dispatcher(cmd, shell);
	free(cmd);
}
// Test 10 : Long pipeline simple ls | grep .c | sort | uniq | wc -l
void test_long_pipeline(t_shell *shell)
{
	printf("\n[Test long pipeline ls | grep .c | sort | uniq | wc -l]\n");
	printf("Attendu : nombre de fichiers .c uniques triés\n");

	char *args1[] = {"ls", NULL};
	char *args2[] = {"grep", ".c", NULL};
	char *args3[] = {"sort", NULL};
	char *args4[] = {"uniq", NULL};
	char *args5[] = {"wc", "-l", NULL};

	t_cmd *cmd1 = create_cmd("ls", args1, STDIN_FILENO, STDOUT_FILENO, shell);
	t_cmd *cmd2 = create_cmd("grep", args2, STDIN_FILENO, STDOUT_FILENO, shell);
	t_cmd *cmd3 = create_cmd("sort", args3, STDIN_FILENO, STDOUT_FILENO, shell);
	t_cmd *cmd4 = create_cmd("uniq", args4, STDIN_FILENO, STDOUT_FILENO, shell);
	t_cmd *cmd5 = create_cmd("wc", args5, STDIN_FILENO, STDOUT_FILENO, shell);

	cmd1->next = cmd2; cmd2->prev = cmd1;
	cmd2->next = cmd3; cmd3->prev = cmd2;
	cmd3->next = cmd4; cmd4->prev = cmd3;
	cmd4->next = cmd5; cmd5->prev = cmd4;

	exec_dispatcher(cmd1, shell);

	free(cmd1);
	free(cmd2);
	free(cmd3);
	free(cmd4);
	free(cmd5);
}

// Test 11 — Pipeline full builtins (plus vicieux) : pwd | echo test | echo again | wc -l
void test_long_pipeline_all_builtins(t_shell *shell)
{
	printf("\n[Test long pipeline pwd | echo test | echo again | wc -l]\n");
	printf("Attendu : probablement 1 (chaîne de builtins avec redirection interne sur wc)\n");

	char *args1[] = {"pwd", NULL};
	char *args2[] = {"echo", "test", NULL};
	char *args3[] = {"echo", "again", NULL};
	char *args4[] = {"wc", "-l", NULL};

	t_cmd *cmd1 = create_cmd("pwd", args1, STDIN_FILENO, STDOUT_FILENO, shell);
	t_cmd *cmd2 = create_cmd("echo", args2, STDIN_FILENO, STDOUT_FILENO, shell);
	t_cmd *cmd3 = create_cmd("echo", args3, STDIN_FILENO, STDOUT_FILENO, shell);
	t_cmd *cmd4 = create_cmd("wc", args4, STDIN_FILENO, STDOUT_FILENO, shell);

	cmd1->next = cmd2; cmd2->prev = cmd1;
	cmd2->next = cmd3; cmd3->prev = cmd2;
	cmd3->next = cmd4; cmd4->prev = cmd3;

	exec_dispatcher(cmd1, shell);

	free(cmd1);
	free(cmd2);
	free(cmd3);
	free(cmd4);
}


// Master runner
void	run_all_tests(t_shell *shell)
{
	test_simple_ls(shell);
	test_single_builtin_pwd(shell);
	test_pipeline_ls_wc(shell);
	test_pipeline_multi(shell);
	test_unknown_cmd(shell);
	test_pipeline_builtin_externe(shell);
	test_pipeline_multi_builtins(shell);
	test_pipeline_with_unknown_cmd(shell);
	// test_wc_without_input(shell);
	test_long_pipeline(shell);
	test_long_pipeline_all_builtins(shell);
}
