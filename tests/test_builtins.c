/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 09:24:37 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/04 13:53:52 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"

void	test_builtin_pwd(t_shell *shell, t_cmd *cmd)
{
	printf("=== TEST PWD ===\n");
	ft_pwd(shell, cmd, 1);
}
void	test_builtin_env(t_shell *shell, t_cmd *cmd)
{
	printf("=== TEST ENV ===\n");
	ft_pwd(shell, cmd, 1);
}
void	test_builtin_echo(t_shell *shell, t_cmd *cmd)
{
	printf("=== TEST ECHO ===\n");
	char *args1[] = {"echo", "hello", "world", NULL};
	char *args2[] = {"echo", "-n", "hello", "again", NULL};
	char *args3[] = {"echo", "-nnnn", "test", NULL};

	// test sans option
	cmd->args = args1;
	ft_echo(shell, cmd, 1);

	// test avec -n
	cmd->args = args2;
	ft_echo(shell, cmd, 1);

	// test avec -nnnn
	cmd->args = args3;
	ft_echo(shell, cmd, 1);
}
void	test_handle_builtin(t_shell *shell, t_cmd *cmd)
{
	printf("=== TEST HANDLE BUILTIN ===\n");
	char *pwd_args[] = {"pwd", NULL};
	char *env_args[] = {"env", NULL};
	char *echo_args[] = {"echo", "coucou", NULL};

	// PWD
	printf("=== TEST PWD ===\n");
	cmd->args = pwd_args;
	handle_builtin(shell, cmd, 1);

	// ENV
	printf("=== TEST ENV ===\n");
	cmd->args = env_args;
	handle_builtin(shell, cmd, 1);

	// ECHO
	printf("=== TEST ECHO ===\n");
	cmd->args = echo_args;
	handle_builtin(shell, cmd, 1);
}

