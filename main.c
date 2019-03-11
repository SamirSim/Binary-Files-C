#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modules.h"
#include "machine_abstraite.h"
#define r 0.5 //taux de chargement du fichier, qui indiquera quand la réorganisation automatique du fichier devra se faire

int main()
{
    LNOVC f;
    char nomfich[tnom]="",nouveau[tnom]="";
    char cle[tcle]="";
    int i,j,trouv;
    float temp;
    while (1==1)
{
    system("cls");
    int ChoixMenu;
    {
        printf("TP2 SFSD : MANIPULATION DES FICHIER LNOVC.\n\n");
        printf("************************MENU**************************\n\n\n\n");
        printf(" 1-CHARGEMENT INITIAL D'UN FICHIER (CREATION D'UN NOUVEAU FICHIER)\n\n");
        printf(" 2-AFFICHAGE DU CONTENU D'UN FICHIER\n\n");
        printf(" 3-RECHERCHE D'UNE DONNEE DANS UN FICHIER\n\n");
        printf(" 4-INSERTION D'UNE DONNEE DANS UN FICHIER \n\n");
        printf(" 5-SUPPRESSION D'UNE DONNEE DANS UN FICHIER\n\n");
        printf(" 6-REORGANISATION D'UN FICHIER \n\n");
        printf(" 7-REORGANISATION D'UN FICHIER AVEC INDEX \n\n");
        printf(" 8-SAUVEGARDE DE L'INDEX DANS UN FICHIER \n\n");
        printf("\n\nQUEL EST VOTRE CHOIX ? ");
        scanf("%d",&ChoixMenu);
        switch (ChoixMenu)
        {
        case 1:
            printf("\nCHARGEMENT INITIAL D'UN FICHIER (CREATION D'UN NOUVEAU FICHIER) :\n\n\n");
            printf("NOM DU FICHIER (AJOUTEZ LE .BIN) : ");
            scanf("%s",nomfich);
            printf("\n\n\n");
            charginit(nomfich);
            printf("VOICI LE CONTENU DE VOTRE FICHIER : \n\n");
            ecritlnovc(nomfich);
            printf("\n\n\n");
            system("pause");
            break;
        case 2:
            printf("\nAFFICHAGE DU CONTENU D'UN FICHIER : \n\n");
            printf("NOM DU FICHIER (AJOUTEZ LE .BIN) : ");
            scanf("%s",nomfich);
            printf("\nLe nom du fichier = %s",nomfich);
            printf("\n\n\n");
            printf("VOICI LE CONTENU DE VOTRE FICHIER : \n\n");
            ecritlnovc(nomfich);
            printf("\n\n\n");
            system("pause");
            break;
        case 3:
            printf("\nRECHERCHE D'UNE DONNEE DANS UN FICHIER : \n\n\n\n");
            printf("NOM DU FICHIER (AJOUTEZ LE .BIN) : ");
            scanf("%s",nomfich);
            printf("\n\n\n");
            printf("ENTREZ LA CLE A RECHERCHER DANS LE FICHIER : ");
            scanf("%s",cle);
            while (strlen(cle)!=tcle)
            {
                printf("\nTAILLE INCOMPATIBLE, VEUILLEZ ENTRER UNE CLE DONT LA TAILLE EST %d : ",tcle);
                scanf("%s",cle);
            }
            printf("\n\n\n");
            recherchelnovc(nomfich,cle,&trouv,&i,&j);
            if (trouv) printf("LA DONNE EXISTE AU BLOC %d ET A LA POSITION %d",i,j);
            else printf("LA DONNEE N'EXISTE PAS ");
            printf("\n\n\n");
            system("pause");
            break;
        case 4:
            printf("\nINSERTION D'UNE DONNEE DANS UN FICHIER : \n\n\n");
            printf("NOM DU FICHIER (AJOUTEZ LE .BIN) : ");
            scanf("%s",nomfich);
            printf("\n\n\n");
            printf("ENTREZ LA CLE A INSERER DANS LE FICHIER : ");
            scanf("%s",cle);
            while (strlen(cle)!=tcle)
            {
                printf("\nTAILLE INCOMPATIBLE, VEUILLEZ ENTRER UNE CLE DONT LA TAILLE EST %d : ",tcle);
                scanf("%s",cle);
            }
            printf("\n\n\n");
            insertionlnovc(nomfich,cle);
            printf("VOICI LE NOUVEAU CONTENU DU FICHIER : \n\n");
            ecritlnovc(nomfich);
            printf("\n\n\n");
            system("pause");
            break;
        case 5:
            printf("\nSUPPRESSION D'UNE DONNEE DANS UN FICHIER : \n\n");
            printf("NOM DU FICHIER (AJOUTEZ LE .BIN) : ");
            scanf("%s",nomfich);
            printf("ENTREZ LA CLE A SUPPRIMER DANS LE FICHIER : ");
            scanf("%s",cle);
            while (strlen(cle)!=tcle)
            {
                printf("\nTAILLE INCOMPATIBLE, VEUILLEZ ENTRER UNE CLE DONT LA TAILLE EST %d : ",tcle);
                scanf("%s",cle);
            }
            printf("\n\n\n");
            suppressionlnovc(nomfich,cle);
            printf("VOICI LE NOUVEAU CONTENU DU FICHIER : \n\n");
            ecritlnovc(nomfich);
            printf("\n\n\n");
            ouvrir(&f,nomfich,"a");
            temp=(entete(&f,2)-1)*b+entete(&f,3);//on multiplie le nombre de blocs dans le fichier (excepté la queue)
                                                 //par la taille d'un bloc, puis on ajoute le nombre de caracteres dans
                                                 //la queue pour obtenir le nombre de caracteres dans le fichier
            if ((entete(&f,4)/temp)>=r)
            {
                printf("\nLE TAUX EST ATTEINT, VEUILLEZ INSERER LE NOM DU FICHIER RESULTAT DE LA REORGANISATION (AJOUTEZ LE .BIN) :  ");
                scanf("%s",nouveau);
                reorganisationlnovc(nomfich,nouveau);
                printf("VOICI LE CONTENU DE VOTRE FICHIER REORGANISE : \n\n");
                ecritlnovc(nouveau);
                printf("\n\n\n");
                printf("\n\n\n");
            }
            fermer(&f);
            system("pause");
            break;
        case 6:
            printf("\nREORGANISATION D'UN FICHIER : \n\n");
            printf("NOM DU FICHIER A REORGANISER (AJOUTEZ LE .BIN) : ");
            scanf("%s",nomfich);
            printf("NOM DU FICHIER RESULTAT (AJOUTEZ LE .BIN) : ");
            scanf("%s",nouveau);
            reorganisationlnovc(nomfich,nouveau);
            printf("VOICI LE CONTENU DE VOTRE FICHIER REORGANISE : \n\n");
            ecritlnovc(nouveau);
            printf("\n\n\n");
            system("pause");
            break;
        case 7:
            printf("\nREORGANISATION D'UN FICHIER AVEC INDEX: \n\n");
            printf("NOM DU FICHIER A REORGANISER (AJOUTEZ LE .BIN) : ");
            scanf("%s",nomfich);
            printf("NOM DU FICHIER RESULTAT (AJOUTEZ LE .BIN) : ");
            scanf("%s",nouveau);
            reorganisation_avec_index(nomfich,nouveau);
            printf("VOICI LE CONTENU DE VOTRE FICHIER REORGANISE : \n\n");
            ecritlnovc(nouveau);
            printf("\n\n\n");
            affiche_index();
            printf("\n\n\n");
            system("pause");
            break;
            case 8:
            printf("\SAUVEGARDE DE L'INDEX DANS UN FICHIER : \n\n");
            printf("NOM DU FICHIER OU SAUVEGARDER (AJOUTEZ LE .BIN) : ");
            scanf("%s",nomfich);
            sauvegarde_idx(nomfich);
            printf("LA SAUVEGARDE A BIEN ETE FAITE.");
            printf("\n\n\n");
            affiche_fich_idx(nomfich);
            printf("\n\n\n");
            system("pause");
            break;
        }
    }
}
}
