/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bool.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 16:35:36 by aumartin          #+#    #+#             */
/*   Updated: 2025/02/27 16:40:57 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "ft_boolean.h"

// Cette fonction prend une condition en paramètre
// Si la condition est vraie (différente de 0), elle renvoie TRUE
// Sinon, elle renvoie FALSE
int	true_or_false(int condition)
{
	if (condition)
		return (TRUE);
	else
		return (FALSE);
}
