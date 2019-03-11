#ifndef MACHINE_ABSTRAITE_H_INCLUDED
#define MACHINE_ABSTRAITE_H_INCLUDED
#define faux 0
#define vrai 1
#define b 70 // taille d'un bloc du fichier
#define tcle 20 // taille de la clé
#define ttaille 3 // taille du champ taille
#define tnom 20
#define TMAX 50
#define b1 10

/* DECLARATION DES TYPES : */
typedef int boolean ;

/* TYPE TBLOC : STRUCTURE DU BUFFER, OU BLOC DANS LE FICHIER */
typedef struct
{
    char tab[b];
    int suiv;
}Tbloc;

/* TYPE TENTETE : STRUCTURE DE L'ENTETE DU FICHIER */
typedef struct
{
    int tete;
    int queue;
    int pos_vide;
    int nb_supp;
}Tentete;

/* TYPE LNOVC : STRUCTURE DU FICHIER : LISTE NON ORDONNE DE TAILLE VARIABLE AVEC CHEVAUCHEMENT */
typedef struct
{
    FILE *fichier;
    Tentete entete;
}LNOVC;

typedef struct
{
    char cl[tcle+1];
    int n_bloc;
    int posi;
}Tindex;

Tindex index[TMAX];
int nbI;//Représente le nombre d'éléments dans l'index

typedef struct
{
    int nb_elem;

}Tentete_idx;


typedef struct
{
    FILE *fichier;
    Tentete_idx entete_idx;
}Fich_index;

/*------------------------------------------------------------------------------------------------*/
/*           Entête des modules d'implémentation du modèle du TP 2 sur les fichiers LNOVC         */
/*------------------------------------ -----------------------------------------------------------*/

/*1*/ void ouvrir(LNOVC *f,char *nomfich,char *mode);

/*2*/ void fermer(LNOVC *f);

/*3*/ void liredir(LNOVC *f,int i,Tbloc *buf);

/*4*/ void ecriredir(LNOVC *f,int i,Tbloc buf);

/*5*/ int entete(LNOVC *f,int i);

/*6*/ void aff_entete(LNOVC *f,int i,int val);

/*7*/ int allocbloc(LNOVC *f);


#endif // MACHINE_ABSTRAITE_H_INCLUDED
