#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "machine_abstraite.h"

/*------------------------------------------------------------------------------------------------*/
/*        Corps des modules d'impl�mentation du mod�le du TP 2 sur les fichiers LNOVC             */
/*------------------------------------ -----------------------------------------------------------*/

/*1*/ void ouvrir(LNOVC *f,char *nomfich,char *mode)//Ouverture d'un fichier en mode nouveau ou ancien
{
    if ((strcmp(mode,"n")==0)||(strcmp(mode,"N")==0))//Fichier inexistant
    {
        f->fichier=fopen(nomfich,"wb+");//Mode d'�criture car nouveau fichier
        //Initialisation de l'entete du fichier
        f->entete.tete=0;
        f->entete.queue=0;
        f->entete.pos_vide=0;
        f->entete.nb_supp=0;
        fseek(f->fichier,0,SEEK_SET);
        fwrite(&f->entete,sizeof(Tentete),1,f->fichier);//Ecriture de l'entete dans le bloc 0 du
        fseek(f->fichier,0,SEEK_SET);                   //fichier pour la sauvegarde en m�moire secondaire
    }
    else
    {
        if ((strcmp(mode,"a")==0)||(strcmp(mode,"A")==0))//Fichier existant
        {
            f->fichier=fopen(nomfich,"rb+");//Mode de lecture car ancien fichier
            if ((f->fichier)!=NULL)
            {
                //R�cuperation de l'ent�te a partir du bloc 0 du fichier (contenant l'ent�te)
                fseek(f->fichier,0,SEEK_SET);
                fread(&f->entete,sizeof(Tentete),1,f->fichier);
                fseek(f->fichier,0,SEEK_SET);
            }
            else
            {
                printf("\nLE FICHIER N'EXISTE PAS.\n");
            }
        }
        else
        {
            printf("\nENTREZ UN MODE D'OUVERTURE VALABLE.\n");
        }
    }
}

/*2*/ void fermer(LNOVC *f)//Fermeture d'un fichier
{
    fclose(f->fichier);
}

/*3*/ void liredir(LNOVC *f,int i,Tbloc *buf)//Recuperation du contenu du bloc i du fichier dans buf
{
    if (f->fichier!=NULL)
    {
        fseek(f->fichier,sizeof(Tentete)+(i-1)*sizeof(Tbloc),SEEK_SET);//Car le fichier contient le bloc 0 (ent�te)
        fread(buf,sizeof(Tbloc),1,f->fichier);//On r�cup�re le bloc de l'adresse correspondante du fichier
        fseek(f->fichier,0,SEEK_SET);
    }
    else
    {
        printf("\nERREUR, FICHIER INEXISTANT.\n");
    }
}

/*4*/ void ecriredir(LNOVC *f,int i,Tbloc buf)//Ecriture du contenu de buf dans le bloc i du fichier
{
    fseek(f->fichier,sizeof(Tentete)+(i-1)*sizeof(Tbloc),SEEK_SET);//Car le fichier contient le bloc 0 (entete)
    fwrite(&buf,sizeof(Tbloc),1,f->fichier);//On �crit le bloc dans le fichier � l'adresse correspondante
    fseek(f->fichier,0,SEEK_SET);
}

/*5*/ int entete(LNOVC *f,int i)//Renvoie l'entete N�i du fichier
{
    switch (i)
    {
        case 1:
            return f->entete.tete;
        break;

        case 2:
            return f->entete.queue;
        break;

        case 3:
            return f->entete.pos_vide;
        break;

        case 4:
            return f->entete.nb_supp;
        break;
    }
    return 0;
}

/*6*/ void aff_entete(LNOVC *f,int i,int val)//Affecte val � l'ent�te N�i du fichier
{
    switch (i)
    {
        case 1:
            f->entete.tete=val;
        break;

        case 2:
            f->entete.queue=val;
        break;

        case 3:
            f->entete.pos_vide=val;
        break;

        case 4:
            f->entete.nb_supp=val;
        break;
    };
    fseek(f->fichier,0,SEEK_SET);//On acc�de au bloc 0 (ent�te)
    fwrite(&f->entete,sizeof(Tentete),1,f->fichier);//Mise � jour de l'entete dans le fichier
    fseek(f->fichier,0,SEEK_SET);
}

/*7*/ int allocbloc(LNOVC *f)//Alloue un bloc dans le fichier, et renvoie son adresse
{
    Tbloc buf;
    ecriredir(f,entete(f,2)+1,buf);//Ajoute un bloc vide au fichier � l'adresse (queue+1)
    aff_entete(f,2,entete(f,2)+1);//Mise � jour de l'entete (queue)
    aff_entete(f,3,0);//Mise � jour de la premi�re position libre
    if (entete(f,1)==0) //Si c'est le 1er bloc � ins�rer (fichier vide)
    {
        aff_entete(f,1,entete(f,1)+1);//On met � jour l'adresse de la nouvelle t�te
    }
    else
    {
        liredir(f,entete(f,2)-1,&buf);//R�cup�ration du dernier bloc (ancienne queue,avant allocation)
        buf.suiv=entete(f,2);//Mise � jour de son suivant (vers le bloc allou�), pour pr�server le cha�nage
        ecriredir(f,entete(f,2)-1,buf);//Ecriture de ce bloc dans le fichier (nouveau suivant)
    }
    return entete(f,2);//On renvoie l'adresse du bloc allou� (nouvelle queue)
}
