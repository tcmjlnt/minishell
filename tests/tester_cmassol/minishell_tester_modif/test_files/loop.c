/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
<<<<<<<< HEAD:tests/funcptr.c
/*   funcptr.c                                          :+:      :+:    :+:   */
========
/*   loop.c                                             :+:      :+:    :+:   */
>>>>>>>> parsing:tester-/minishell_tester_modif/test_files/loop.c
/*                                                    +:+ +:+         +:+     */
/*   By: lalex-ku <lalex-ku@42sp.org.br>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<<< HEAD:tests/funcptr.c
/*   Created: 2025/06/05 10:30:42 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/05 10:37:01 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* apprentissage avec Gaelle du tableau de struc (= tableau d'objets) */
/* int	fa(void)
{
	return (1);
========
/*   Created: 2022/06/22 17:32:45 by lalex-ku          #+#    #+#             */
/*   Updated: 2022/06/22 17:32:50 by lalex-ku         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int	main(int argc, char const *argv[])
{
	int	pid;

	pid = fork();
	open("infile", O_RDONLY);
	while (1)
	{
		printf("Helloo miniHELL %i\n", pid);
		sleep(1);
	}
	return (0);
>>>>>>>> parsing:tester-/minishell_tester_modif/test_files/loop.c
}

int	fb(void)
{
	return (2);
}

int	fc(void)
{
	return (3);
}

typedef int	(*fptr)(void);

fptr	*cpnd(void)
{
	fptr	*func;

	func = (fptr []){fa, fb, fc};
	return (func);
}

int main()
{
	cpnd();
} */

// int main() {
// 	int *blbl;

// 	blbl = (int []){1, 2, 3};
// }
