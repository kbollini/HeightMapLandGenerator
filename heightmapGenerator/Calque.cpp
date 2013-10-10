#include "stdafx.h"
#include "Calque.h"
#include <iostream>
#include <fstream>

using namespace std;

Calque::Calque(int t, float p){

	this->v = new int*[t];
    int i,j;
    for (i=0; i<t ; i++)
	{
        this->v[i]= new int[t];
        for (j=0; j<t; j++)
            this->v[i][j]=0;
    }
    this->taille = t;
    this->persistance = p;

}

Calque::~Calque(){
    int j;
    for (j=0; j<this->taille; j++)
        delete(this->v[j]);
    delete(this->v);
}

void Calque::generer_calque(int frequence,int octaves,float persistance,int liss,char *filename)
{
    // itératif
    int taille = this->taille;
    int i,j,n,f_courante;
    int x,y,k,l;
    int a;
    float pas, sum_persistances;

    pas = (float)(taille)/frequence;
    float persistance_courante = persistance;

    // calque aléatoire
    Calque random (taille, 1);
    for (i=0; i<taille; i++)
        for (j=0; j<taille; j++)
		{
			random.v[i][j]=aleatoire(256);//(float)rand() / RAND_MAX * a;
		}

	//si on veut enregistrer
	//enregistrer_raw("alea.raw");

	/* calques de travail
	 * TODO : faire un tableau de taille adaptée
	*/
    //Calque *mes_calques[octaves];
	Calque *mes_calques[10];
    for (i=0; i<octaves; i++){
        mes_calques[i] = new Calque(taille,persistance_courante);
        persistance_courante*=persistance;
    }

    f_courante = frequence;

    // remplissage de calque
    for (n=0; n<octaves; n++){
        for(i=0; i<taille; i++)
            for(j=0; j<taille; j++) {
                a = random.valeur_interpolee(i, j, f_courante);
                mes_calques[n]->v[i][j]=a;
            }
        f_courante*=frequence;
    }

    sum_persistances = 0;
    for (i=0; i<octaves; i++)
        sum_persistances+=mes_calques[i]->persistance;

    // ajout des calques successifs
    for (i=0; i<taille; i++)
        for (j=0; j<taille; j++){
            for (n=0; n<octaves; n++)
				this->v[i][j]+=mes_calques[n]->v[i][j]*mes_calques[n]->persistance;

            // normalisation
            this->v[i][j] =  this->v[i][j] / sum_persistances;
        }


    // lissage
    Calque lissage(taille, 0);

    for (x=0; x<taille; x++)
        for (y=0; y<taille; y++){
            a=0;
            n=0;
            for (k=x-liss; k<=x+liss; k++)
                for (l=y-liss; l<=y+liss; l++)
                    if ((k>=0) && (k<taille) && (l>=0) && (l<taille)) {
                        n++;
                        a+=this->v[k][l];
                    }
					lissage.v[x][y] = (float)a/n;
        }

	//si on veut enregistrer 
	lissage.enregistrer_raw(filename);


    // TODO mémoire libération
/*    delete random;
    delete lissage;
    for (i=0; i<octaves; i++)
        delete(mes_calques[i]);
    delete(mes_calques);*/
}

int Calque::interpolate(int y1, int y2, int n, int delta){

    // interpolation non linéaire
    if (n==0)
        return y1;
    if (n==1)
        return y2;

    float a = (float)delta/n;

    float fac1 = 3*pow(1-a, 2) - 2*pow(1-a,3);
    float fac2 = 3*pow(a, 2) - 2*pow(a, 3);

    return y1*fac1 + y2*fac2;

	//////////////////////////////////////////////

    // interpolation linéaire
    /*if (n!=0)
        return y1+delta*((float)y2-(float)y1)/(float)n;
    else
        return y1;*/
}


int Calque::valeur_interpolee(int i, int j, int frequence){
    // valeurs des bornes
    int borne1x, borne1y, borne2x, borne2y, q;
    float pas;
	pas = (float)this->taille/frequence;

    q = (float)i/pas;
    borne1x = q*pas;
    borne2x = (q+1)*pas;

    if(borne2x >= this->taille)
        borne2x = this->taille-1;

    q = (float)j/pas;
    borne1y = q*pas;
    borne2y = (q+1)*pas;

    if(borne2y >= this->taille)
        borne2y = this->taille-1;

    int b00,b01,b10,b11;
    b00 = this->v[borne1x][borne1y];
    b01 = this->v[borne1x][borne2y];
    b10 = this->v[borne2x][borne1y];
    b11 = this->v[borne2x][borne2y];

    int v1 = interpolate(b00, b01, borne2y-borne1y, j-borne1y);
    int v2 = interpolate(b10, b11, borne2y-borne1y, j-borne1y);
    int fin = interpolate(v1, v2, borne2x-borne1x , i-borne1x);

    return fin;
}

// fournit une valeur entre 0 et a
unsigned char Calque::aleatoire(float a){
    return (float)rand() / RAND_MAX * a;
}


void Calque::enregistrer_raw(char *filename)
{
	 // ouverture en écriture avec effacement du fichier ouvert
	ofstream fichier(filename, ios::out | ios::trunc); 

	if (!fichier)
		 cerr << "Impossible d'ouvrir le fichier !" << endl;
	else{
		int i,j;
		for (i=0; i<this->taille; i++)
			for (j=0; j<this->taille; j++){
				fichier << (unsigned char)this->v[i][j];
			}
		fichier.close();
	}
}