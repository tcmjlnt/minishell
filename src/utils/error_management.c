/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 18:33:46 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/27 11:35:40 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	error_exit(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

void	error_free_gc(const char *message)
{
	gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE); // GC_NONE ou GC_CMD
	error_exit(message);
}
void	error_free_exit(const char *message)
{
	perror(message);
	gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);
	exit(EXIT_FAILURE);
}


// a implemnter ??
int	get_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
