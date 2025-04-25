// Initialiser les structures globales (environnement, garbage collector, etc.)
// Gérer la boucle principale du shell : affichage du prompt, lecture ligne, parsing, exécution
// Libérer toutes les ressources à la fin (gc_free_all avec GC_NONE)


#include <unistd.h>

int main()
{
	write(1, "Let's goooooo", 14);
	return 0;
}
