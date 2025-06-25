/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_exec_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 08:32:20 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/25 22:12:16 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

t_cmd	*create_cmd(char *cmd_name, char **args, int fd_in, int fd_out, t_shell *shell)
{
	t_cmd		*cmd;

	cmd = gc_mem(GC_ALLOC, sizeof(t_cmd), NULL, GC_CMD);
	if (!cmd)
		return (NULL);
	cmd->cmd = cmd_name;
	cmd->args = args;
	cmd->fd_in = fd_in;
	cmd->fd_out = fd_out;
	cmd->is_builtin = is_builtin(shell, cmd_name);
	cmd->next = NULL;
	cmd->prev = NULL;
	return (cmd);
}

void	add_cmd(t_cmd **head, t_cmd *new_cmd)
{
	t_cmd	*current;

	if (!head || !new_cmd)
		return ;
	if (*head == NULL)
	{
		*head = new_cmd;
		return ;
	}
	current = *head;
	while (current->next != NULL)
		current = current->next;
	current->next = new_cmd;
	new_cmd->prev = current;
}

t_redir *create_redir(t_token_type type, char *file_name)
{
	t_redir *redir = gc_mem(GC_ALLOC, sizeof(t_redir), NULL, GC_CMD);
	redir->type = type;
	redir->file = gc_strdup(file_name, GC_CMD);
	redir->next = NULL;
	return (redir);
}
// Test simple output >
void test_output_redir(t_shell *shell)
{
	char *args[] = {"echo", "hello redir", NULL};
	t_cmd *cmd = create_cmd("echo", args, STDIN_FILENO, STDOUT_FILENO, shell);

	cmd->redir = create_redir(TKN_OUT, "out_test1.txt");

	printf("\n[Test redirection >]\n");
	exec_dispatcher(cmd, shell);

}
// Test append >>
void test_append_redir(t_shell *shell)
{
	char *args[] = {"echo", "appending text", NULL};
	t_cmd *cmd = create_cmd("echo", args, STDIN_FILENO, STDOUT_FILENO, shell);

	cmd->redir = create_redir(TKN_APPEND, "out_test1.txt");

	printf("\n[Test redirection >>]\n");
	exec_dispatcher(cmd, shell);

}

// Test input <
void test_input_redir(t_shell *shell)
{
	char *args[] = {"cat", NULL};
	t_cmd *cmd = create_cmd("cat", args, STDIN_FILENO, STDOUT_FILENO, shell);

	cmd->redir = create_redir(TKN_IN, "input_test.txt");

	printf("\n[Test redirection <]\n");
	exec_dispatcher(cmd, shell);

}

// Test multiple redirections (input + output)
void test_in_out_redir(t_shell *shell)
{
	char *args[] = {"cat", NULL};
	t_cmd *cmd = create_cmd("cat", args, STDIN_FILENO, STDOUT_FILENO, shell);

	t_redir *redir_in = create_redir(TKN_IN, "input_test.txt");
	t_redir *redir_out = create_redir(TKN_OUT, "test2.txt");

	redir_in->next = redir_out;
	cmd->redir = redir_in;

	printf("\n[Test redirection multiple < et >]\n");
	exec_dispatcher(cmd, shell);

}

// Test erreur fichier inexistant <
void test_input_error(t_shell *shell)
{
    char *args[] = {"cat", NULL};
    t_cmd *cmd = create_cmd("cat", args, STDIN_FILENO, STDOUT_FILENO, shell);

    cmd->redir = create_redir(TKN_IN, "fichier_inexistant.txt");

    printf("\n[Test redirection < fichier inexistant]\n");
    exec_dispatcher(cmd, shell);

}

// Test pipe + redirection
void test_pipe_redir(t_shell *shell)
{
    // Simule : cat < input.txt | grep mot > result.txt
    char *args1[] = {"cat", NULL};
    char *args2[] = {"grep", "test", NULL};
    t_cmd *cmd1 = create_cmd("cat", args1, STDIN_FILENO, STDOUT_FILENO, shell);
    t_cmd *cmd2 = create_cmd("grep", args2, STDIN_FILENO, STDOUT_FILENO, shell);

    cmd1->redir = create_redir(TKN_IN, "input_test.txt");
    cmd2->redir = create_redir(TKN_OUT, "result_test.txt");

    add_cmd(&cmd1, cmd2);

    printf("\n[Test pipe + redirection]\n");
    exec_dispatcher(cmd1, shell);

}


void run_redirections_tests(t_shell *shell)
{
	test_output_redir(shell);
	test_append_redir(shell);
	test_input_redir(shell);
	test_in_out_redir(shell);
	test_input_error(shell);
	test_pipe_redir(shell);

}

