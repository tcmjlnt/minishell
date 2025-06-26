/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_codes.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:30:00 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/26 08:23:22 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXIT_CODES_H
# define EXIT_CODES_H

/* ========================================================================== */
/*                           📊 EXIT STATUS CODES                            */
/* ========================================================================== */

/* Standard success and error codes */
# define EXIT_SUCCESS				0	// Succès
# define EXIT_GENERAL_ERROR			1	// Erreur générale
# define EXIT_MISUSE_BUILTIN		2	// Mauvais usage de builtin

/* Command execution errors */
# define EXIT_CANNOT_EXECUTE		126	// Permission denied, is directory
# define EXIT_COMMAND_NOT_FOUND		127	// Command not found
# define EXIT_INVALID_EXIT_ARG		128	// Invalid argument to exit

/* Signal-related exit codes */
# define EXIT_SIGINT				130	// SIGINT (Ctrl+C)
# define EXIT_SIGQUIT				131	// SIGQUIT (Ctrl+\)

/* Signal formula: 128 + signal_number */
# define EXIT_SIGNAL_BASE			128

/* Specific error contexts */
# define EXIT_FORK_ERROR			1	// Fork failed
# define EXIT_PIPE_ERROR			1	// Pipe creation failed
# define EXIT_REDIR_ERROR			1	// Redirection error
# define EXIT_HEREDOC_ERROR			1	// Heredoc processing error

/* Builtin-specific codes */
# define EXIT_CD_ERROR				1	// cd: directory not found, permission denied
# define EXIT_EXPORT_ERROR			1	// export: invalid identifier
# define EXIT_UNSET_ERROR			1	// unset: invalid identifier
# define EXIT_EXIT_TOO_MANY_ARGS	1	// exit: too many arguments
# define EXIT_EXIT_NON_NUMERIC		2	// exit: numeric argument required

/* File and permission errors */
# define EXIT_FILE_NOT_FOUND		127	// File not found
# define EXIT_PERMISSION_DENIED		126	// Permission denied
# define EXIT_IS_DIRECTORY			126	// Attempted to execute directory

/* ========================================================================== */
/*                              🛠️ UTILITY MACROS                            */
/* ========================================================================== */

/* Check if exit code indicates signal termination */
# define IS_SIGNAL_EXIT(code)		((code) >= EXIT_SIGNAL_BASE)

/* Extract signal number from exit code */
# define GET_SIGNAL_NUM(code)		((code) - EXIT_SIGNAL_BASE)

/* Create exit code from signal number */
# define SIGNAL_TO_EXIT(sig)		(EXIT_SIGNAL_BASE + (sig))

/* Check for specific signal exits */
# define IS_SIGINT_EXIT(code)		((code) == EXIT_SIGINT)
# define IS_SIGQUIT_EXIT(code)		((code) == EXIT_SIGQUIT)

/* ========================================================================== */
/*                            📝 USAGE DOCUMENTATION                         */
/* ========================================================================== */

/*
 * EXIT CODE USAGE GUIDE:
 *
 * 1. SUCCESS (0):
 *    - Command executed successfully
 *    - All operations completed without error
 *
 * 2. GENERAL ERROR (1):
 *    - Fork, pipe, redirection failures
 *    - Builtin argument errors (cd, export, unset)
 *    - File permission issues
 *
 * 3. MISUSE (2):
 *    - exit with non-numeric argument
 *    - Reserved for builtin misuse
 *
 * 4. CANNOT EXECUTE (126):
 *    - Permission denied on executable
 *    - Attempting to execute directory
 *    - File is not executable
 *
 * 5. COMMAND NOT FOUND (127):
 *    - Command not in PATH
 *    - Command does not exist
 *    - Typo in command name
 *
 * 6. SIGNAL EXITS (128+):
 *    - Process terminated by signal
 *    - 130: SIGINT (Ctrl+C)
 *    - 131: SIGQUIT (Ctrl+\)
 *    - Formula: 128 + signal_number
 */

#endif
