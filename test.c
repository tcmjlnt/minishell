/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:28:44 by tjacquel          #+#    #+#             */
/*   Updated: 2025/05/31 20:28:33 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <limits.h>

long	ft_atol(const char *nptr)
{
	long long	i;
	long long	res;
	int			sign;

	i = 0;
	res = 0;
	sign = 1;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] && nptr[i] >= '0' && nptr[i] <= '9')
	{
		if (res > (LLONG_MAX - (nptr[i] - '0')) / 10)
			return (LONG_MAX);
		res = 10 * res + (nptr[i] - '0');
		i++;
	}
	if (res > LONG_MAX || - res < LONG_MIN)
		return (LONG_MAX);
	return ((long)(sign * res));
}
int	main(int argc, char **argv)
{
	long res;
	if (argc == 2)
	{
		res = ft_atol(argv[1]);
		printf("atol res is: %ld\n", res);
	}

	return (0);

}
