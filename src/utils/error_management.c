/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 18:33:46 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/27 18:11:10 by tjacquel         ###   ########.fr       */
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
	gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);
	error_exit(message);
}
void	free_gc_exit(void)
{
	gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);
	exit(EXIT_FAILURE);
}
