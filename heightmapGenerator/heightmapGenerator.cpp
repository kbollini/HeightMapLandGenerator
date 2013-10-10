#include "stdafx.h"
#include <time.h>

int main(int argc, char **argv)
{
    srand((int)time(NULL));
    //srand(6);

    // valeurs d'entrée
	int octaves=3;
	int frequence=4;
	float persistence=.5;
    int taille=200;
    int lissage = 3;

    // création de calque
    Calque calque (taille, 1);

	calque.generer_calque(frequence,octaves,persistence,lissage,"resultat.raw");

	return 0;
}