/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status_manager.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:35:00 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/26 08:25:43 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../exit_codes.h"

/* ========================================================================== */
/*                          🎯 CENTRALIZED EXIT STATUS                       */
/* ========================================================================== */

/**
 * Set exit status with optional error message
 * @param shell     Pointer to shell structure
 * @param code      Exit code to set
 * @param context   Optional context string for debugging
 * @param message   Optional error message to display
 */
void	set_exit_status(t_shell *shell, int code, const char *context, const char *message)
{
	shell->exit_status = code;

	if (message)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		if (context)
		{
			ft_putstr_fd(context, STDERR_FILENO);
			ft_putstr_fd(": ", STDERR_FILENO);
		}
		ft_putstr_fd(message, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
}

/**
 * Extract exit status from wait() status
 * @param status    Status returned by wait()
 * @return          Appropriate exit code
 */
int	extract_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		int sig = WTERMSIG(status);
		return (SIGNAL_TO_EXIT(sig));
	}
	return (EXIT_GENERAL_ERROR);
}

/**
 * Handle signal-specific behavior (messages, etc.)
 * @param shell     Pointer to shell structure
 * @param exit_code Exit code to check for signal behavior
 */
void	handle_signal_behavior(t_shell *shell, int exit_code)
{
	(void)shell;

	if (IS_SIGINT_EXIT(exit_code))
	{
		write(STDERR_FILENO, "\n", 1);
	}
	else if (IS_SIGQUIT_EXIT(exit_code))
	{
		write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	}
}

/**
 * Set command not found error
 * @param shell   Pointer to shell structure
 * @param cmd     Command that was not found
 */
void	set_command_not_found(t_shell *shell, const char *cmd)
{
	set_exit_status(shell, EXIT_COMMAND_NOT_FOUND, cmd, "command not found");
}

/**
 * Set permission denied error
 * @param shell   Pointer to shell structure
 * @param file    File that caused permission error
 */
void	set_permission_denied(t_shell *shell, const char *file)
{
	set_exit_status(shell, EXIT_CANNOT_EXECUTE, file, "Permission denied");
}

/**
 * Set directory execution error
 * @param shell   Pointer to shell structure
 * @param dir     Directory that was attempted to execute
 */
void	set_is_directory(t_shell *shell, const char *dir)
{
	set_exit_status(shell, EXIT_CANNOT_EXECUTE, dir, "Is a directory");
}

/**
 * Set general error with context
 * @param shell     Pointer to shell structure
 * @param context   Context where error occurred
 * @param message   Error message
 */
void	set_general_error(t_shell *shell, const char *context, const char *message)
{
	set_exit_status(shell, EXIT_GENERAL_ERROR, context, message);
}

/**
 * Set builtin misuse error
 * @param shell     Pointer to shell structure
 * @param builtin   Name of the builtin
 * @param message   Specific error message
 */
void	set_builtin_error(t_shell *shell, const char *builtin, const char *message)
{
	set_exit_status(shell, EXIT_MISUSE_BUILTIN, builtin, message);
}

/* ========================================================================== */
/*                            🧪 DIAGNOSTIC FUNCTIONS                        */
/* ========================================================================== */

/**
 * Get human-readable description of exit code
 * @param code  Exit code to describe
 * @return      Static string describing the exit code
 */
const char	*get_exit_code_description(int code)
{
	if (code == EXIT_SUCCESS)
		return ("Success");
	else if (code == EXIT_GENERAL_ERROR)
		return ("General error");
	else if (code == EXIT_MISUSE_BUILTIN)
		return ("Builtin misuse");
	else if (code == EXIT_CANNOT_EXECUTE)
		return ("Cannot execute");
	else if (code == EXIT_COMMAND_NOT_FOUND)
		return ("Command not found");
	else if (IS_SIGINT_EXIT(code))
		return ("Interrupted (SIGINT)");
	else if (IS_SIGQUIT_EXIT(code))
		return ("Quit (SIGQUIT)");
	else if (IS_SIGNAL_EXIT(code))
		return ("Terminated by signal");
	else
		return ("Unknown exit code");
}

/**
 * Debug function to print current exit status
 * @param shell   Pointer to shell structure
 */
void	debug_exit_status(t_shell *shell)
{
	ft_putstr_fd("[DEBUG] Exit status: ", STDERR_FILENO);
	ft_putnbr_fd(shell->exit_status, STDERR_FILENO);
	ft_putstr_fd(" (", STDERR_FILENO);
	ft_putstr_fd(get_exit_code_description(shell->exit_status), STDERR_FILENO);
	ft_putstr_fd(")\n", STDERR_FILENO);
}
