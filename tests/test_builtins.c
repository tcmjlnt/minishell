/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 09:24:37 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/04 09:56:59 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"

void	test_builtin_pwd(t_shell *shell)
{
	printf("=== TEST PWD ===\n");
	ft_pwd(shell, 1);
}
void	test_builtin_env(t_shell *shell)
{
	printf("=== TEST ENV ===\n");
	ft_env(shell, 1);
}
void	test_builtin_echo(t_shell *shell)
{
	printf("=== TEST ECHO ===\n");
	char *args1[] = {"echo", "hello", "world", NULL};
	char *args2[] = {"echo", "-n", "hello", "again", NULL};
	char *args3[] = {"echo", "-nnnn", "test", NULL};

	// test sans option
	shell->current_cmd->args = args1;
	ft_echo(shell, 1);

	// test avec -n
	shell->current_cmd->args = args2;
	ft_echo(shell, 1);

	// test avec -nnnn
	shell->current_cmd->args = args3;
	ft_echo(shell, 1);
}
void	test_handle_builtin(t_shell *shell)
{
	printf("=== TEST HANDLE BUILTIN ===\n");
	char *pwd_args[] = {"pwd", NULL};
	char *env_args[] = {"env", NULL};
	char *echo_args[] = {"echo", "coucou", NULL};

	// PWD
	printf("=== TEST PWD ===\n");
	shell->current_cmd->args = pwd_args;
	handle_builtin(shell, 1);

	// ENV
	printf("=== TEST ENV ===\n");
	shell->current_cmd->args = env_args;
	handle_builtin(shell, 1);

	// ECHO
	printf("=== TEST ECHO ===\n");
	shell->current_cmd->args = echo_args;
	handle_builtin(shell, 1);
}

