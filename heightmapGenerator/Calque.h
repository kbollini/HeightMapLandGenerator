#pragma once

class Calque {

public : 

    int **v;
    int taille;
    float persistance;

	Calque();
	Calque (int t, float f);
	~Calque();
	virtual void generer_calque(int frequence, int octaves,float persistance,int liss, char *filename);
	virtual int interpolate(int, int, int, int);
	virtual int valeur_interpolee(int, int, int);
	virtual unsigned char aleatoire(float a);

	virtual void enregistrer_raw( char *filename);
};