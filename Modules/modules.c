#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conio.h"
#include "machine_abstraite.h"
#include "modules.h"

/*------------------------------------------------------------------------------------------------*/
/*                     Corps des modules du TP 2 sur les fichiers LNOVC                        */
/*------------------------------------ -----------------------------------------------------------*/

/** 1-  **************************************************************/
void recherchelnovc(char *nomfich,char cle[tcle],int *trouv,int *i,int *j)
//Recherche de la cle dans le fichier, avec trouv=1 si la cle existe, 0 sinon , et i renvoie
//le N° du bloc où se trouve la cle si elle existe, sinon le N° du bloc ou elle devrait être insérée,
//et j la position où se rouve la cle si elle existe, sinon la position ou elle devrait être insérée
{
    LNOVC f;
    Tbloc buf;
    int k,p,tai,temp=0,m;
    char taille[ttaille],chaine[tcle];
    ouvrir(&f,nomfich,"a");
    *trouv=0;
    *i=1;
    if (f.fichier!=NULL)
    {
        p=entete(&f,1);//On récupère l'adresse de la tete du fichier
        k=0;//k va représenter la variable de parcours du bloc
        liredir(&f,p,&buf);//On récupère le bloc de la tête
        while ((p!=-1)&&(*trouv==0))
        {
            temp=k;//On affecte à temp la position courante
            for (m=1;m<=ttaille;m++)
            {
                if (k>=b)//Si la position courante dépasse la taille du bloc
                {
                    k=0;
                    *i=p;//On affecte à i le bloc courant, car le bloc où se trouve la clé représente
                         //le bloc où se trouve le début de la donnée, or la clé peut se trouver dans
                         //un bloc alors que la donnée commence dans le bloc précédent
                    p=buf.suiv;//On passe au bloc suivant
                    liredir(&f,p,&buf);
                }
                taille[m-1]=buf.tab[k];//On récupère le champ taille qui se trouve dans le fichier
                k++;
            }
            taille[ttaille]='\0';//Pour indiquer la fin du champ 'taille'
            tai=atoi(taille);//On convertit la chaine récupérée en entier
            k++;//On passe à la position suivante, car le champ 'effacé' ne nous intéresse pas ici
            if (k>=b)//A chaque fois qu'on incrémente la position courante, on doit vérifier si elle
                    //ne dépasse pas la taille du bloc
            {
                k=0;
                p=buf.suiv;
                liredir(&f,p,&buf);
            }
            for (m=1;m<=tcle;m++)
            {   //On récupère le champ cle qui se trouve dans le fichier
                if (k>=b)
                {
                    k=0;
                    p=buf.suiv;
                    liredir(&f,p,&buf);
                }
                chaine[m-1]=buf.tab[k];
                k++;
            }
            chaine[tcle]='\0';//Pour indiquer la fin de la clé récupérée
            cle[tcle]='\0';//Pour indiquer la fin de la clé recherchée
            if (strcmp(chaine,cle)==0)//On compare entre la cle récupérée du fichier et la clé recherchée
            {
                *trouv=1;//La clé est identique, donc clé trouvée
            }
            else
            {
                k=k+tai-(tcle+4);//Clé non trouvée, donc on passe à l'info suivante en ajoutant à la position
                                 //courante la taille de l'info précédente-(tcle+24), car la clé et les
                                 //3 premiers caractères ne sont pas comptabilisés
                if (k>=b)
                {   //On passe au début de la donnée suivante, dans le bloc suivant
                    k=k%b;
                    p=buf.suiv;
                    liredir(&f,p,&buf);
                }
                *i=p;//On affecte à i l'adresse du bloc courant
            }
            if ((p==entete(&f,2))&&(k==entete(&f,3))) // On est arrivés à la première position vide du fichier
            {
                p=-1; // Pour sortir de la boucle
            }
        }
    }
    *j=temp;//On affecte à j temp qui représente la position de la donnée dans le bloc
     fermer(&f);
}

/** 2-  *************************************************************************************/
void insertionlnovc(char *nomfich,char cle[tcle])
{
    LNOVC f;
    Tbloc buf,buf1,buf2;//buf et buf2 sont déclarées ici malgré leur inutilité dans le programme car les enlever causerait
                        //des erreurs au niveau de l'éxécution
    char info[]="";//La donnée de taille variable, nous n'avons pas utilisé de pointeur car ça cause des problèmes à la lecture
    int i,j,trouv,taille,m,q,p,k,tai;
    char chaine[ttaille],tail[ttaille];
    recherchelnovc(nomfich,cle,&trouv,&i,&j);//On recherche la clé dans le fichier
    ouvrir(&f,nomfich,"a");
    if (trouv==1)//Cle existante dans le fichier
    {
        liredir(&f,i,&buf1);//On lit le bloc i du fichier (ou se trouve la cle trouvée)
        for (m=1;m<=ttaille;m++)//On récupère la taille de la donnée pour la mise à jour de l'entête du nombre de caractères
                                //supprimés, si jamais on souhaite insérer une donnée supprimée
        {
            if (j>=b)//Si la position courante dépasse la taille du bloc
            {
                j=0;
                i=buf1.suiv;//On passe au bloc suivant
                liredir(&f,i,&buf1);
            }
            tail[m-1]=buf1.tab[j];//On récupère le champ taille qui se trouve dans le fichier
            j++;
        }
        tail[ttaille]='\0';//Pour indiquer la fin du champ 'taille'
        tai=atoi(tail);//On convertit la chaine récupérée en entier
        if (j>=b)//Si le champ effacé de la donnée se trouve dans le bloc suivant
        {
            i=buf1.suiv;//On passe au bloc suivant
            liredir(&f,i,&buf1);
        }
        j=j%b;//On récupère le champ 'effacé' de la donnée
        if (buf1.tab[j]=='1')//Si elle a été supprimée
        {
            buf1.tab[j]='0';//On la remet à '0' pour dire qu'elle n'est plus supprimée
            ecriredir(&f,i,buf1);//On met à jour le bloc dans le fichier
            aff_entete(&f,4,entete(&f,4)-tai);//On met à jour le nombre de caractères supprimées dans l'entête du fichier
        }
        else
        {
            printf("\nDONNEE EXISTE DEJA.\n");
        }
    }
    else
    {
        p=entete(&f,2);//On récupère la queue du fichier
        if (p!=0)//Si le fichier n'est pas vide
        {
            liredir(&f,p,&buf1);//On récupère le bloc de la queue
        }
        printf("\nENTREZ LA DONNEE A INSERER : ");
        scanf("%s",info);//On récupère l'info à insérer
        printf("\nLA DONNEE EST : %s\n",info);
        taille=strlen(info);//On récupère la longueur de l'info
        itoa(taille+tcle+4,chaine,10);//On convertit la taille en chaine de caractères pour l'insertion
                                      //dans le buffer (taille de l'info + tcle + 3 du champ 'taille'
                                      // + 1 du champ 'effacé'
        chaine[ttaille]='\0';
        if (taille<100)//Pour que les trois positions du champ 'taille' soient toutes pleines
        {
            chaine[2]=chaine[1];
            chaine[1]=chaine[0];
            chaine[0]='0';
        }
        k=entete(&f,3);//On récupère la première position vide du fichier, pour y insérer la donnée
        for (m=1;m<=taille+tcle+4;m++)
        {
            if (k>=b)
            {
                k=0;//On passe à la première position du bloc suivant
                q=p;//On sauvegarde l'adresse du bloc précédent
                p=allocbloc(&f);//On alloue un nouveau bloc au fichier pour y insérer la donnée
                aff_entete(&f,2,p);//On met à jour la nouvelle queue dans l'entête
                buf1.suiv=p;//On chaîne l'ancienne queue avec la nouvelle
                ecriredir(&f,q,buf1);//On met à jour l'ancienne queue dans le fichier
                aff_entete(&f,3,k);//On met à jour l'adresse de la queue dans l'entête
            }
            if ((m>=1)&&(m<=3))
            {
                buf1.tab[k]=chaine[m-1];//On remplit le champ 'taille' avec la taille de la donnée
            }
            if (m==4)
            {
                buf1.tab[k]='0';//On met la champ 'effacé' à '0'
            }
            if ((m>=5)&&(m<=24))
            {
                buf1.tab[k]=cle[m-5];//On remplit la champ 'clé' avec la clé à insérer
            }
            if (m>=25)
            {
                buf1.tab[k]=info[m-25];//On remplit le champ 'donnée' avec la donnée à insérer
            }
            k++;//Le remplissage se fait caractère par caractère, donc on passe à la position suivante
                //dans buf
        }
        buf1.suiv=-1;//On chaîne la nouvelle queue à NIL
        ecriredir(&f,p,buf1);//On écrit la dernière queue dans le fichier
        aff_entete(&f,3,k); //On met à jour la première position vide, et on met k à sa place
                            //car le k pointe la dérnière position insérée +1 . Si le bloc est plein on aura j+1=b+1
    }
    fermer(&f);
}

/** 3-  ************************************************************************************/
void reorginsertlnovc(char *nomfich,char taille[ttaille],char cle[tcle],char info[b]) // Insertion sans faire la recherche car on est sûr que l'élément n'existe pas
//Ce module nous sera utile dans la réorganisation du fichier, ainsi ça ne sera pas nécéssaire de rechercher la clé vu qu'on sait
//qu'elle n'existe pas dans le fichier
{
    LNOVC f;
    Tbloc buf1;
    int m,q,p,k,tai;
    ouvrir(&f,nomfich,"a");
    p=entete(&f,2);//On récupère la queue du fichier
    if (p!=0)
    {
        liredir(&f,p,&buf1);//On récupère le bloc de la queue
    }
    else
    {
        p=allocbloc(&f);//On alloue un nouveau bloc au fichier pour y insérer la donnée
    }
    tai=atoi(taille);//On convertit la chaine récupérée en entier
    k=entete(&f,3);//On récupère la première position vide du fichier, pour y insérer la donnée
    for (m=1;m<=tai;m++)
    {
        if (k>=b)
        {
            k=0;//On passe à la première position du bloc suivant
            q=p;//On sauvegarde l'adresse du bloc précédent
            p=allocbloc(&f);//On alloue un nouveau bloc au fichier pour y insérer la donnée
            aff_entete(&f,2,p);//On met à jour la nouvelle queue dans l'entête
            buf1.suiv=p;//On chaîne l'ancienne queue avec la nouvelle
            ecriredir(&f,q,buf1);//On met à jour l'ancienne queue dans le fichier
            aff_entete(&f,3,k);//On met à jour la première position vide du fichier dans l'entête, si jamais on a déjà
                                //inséré toute la donnée
        }
        if ((m>=1)&&(m<=3))
        {
            buf1.tab[k]=taille[m-1];//On remplit le champ 'taille' avec la taille de la donnée
        }
        if (m==4)
        {
            buf1.tab[k]='0';//On met la champ 'effacé' à '0'
        }
        if ((m>=5)&&(m<=24))
        {
            buf1.tab[k]=cle[m-5];//On remplit la champ 'clé' avec la clé à insérer
        }
        if (m>=25)
        {
            buf1.tab[k]=info[m-25];//On remplit le champ 'donnée' avec la donnée à insérer
        }
        k++;//Le remplissage se fait caractère par caractère, donc on passe à la position suivante
            //dans buf
    }
    buf1.suiv=-1;
    ecriredir(&f,p,buf1);//On écrit le bloc dans le fichier
    aff_entete(&f,3,k); //On met à jour la première position vide, et on met k à sa place
                        //car le k (sortant de la boucle) pointe la dérnière position insérée +1, donc la première position vide.
                        //Si le bloc est plein on aura k+1=b+1
    fermer(&f);
}

/** 4-  **********************************************************************************************/
void suppressionlnovc(char *nomfich,char cle[tcle])
//Effectue la suppression logique de la donnée dont la clé est 'cle' dans le fichier
{
    LNOVC f;
    Tbloc buf;
    int tai,m,i,j,trouv;
    char taille[ttaille];
    recherchelnovc(nomfich,cle,&trouv,&i,&j);//On recherche la donnée dans le fichier
    if (trouv==1)// Si donnée trouvée
    {
        ouvrir(&f,nomfich,"a");
        liredir(&f,i,&buf);//On récupère le bloc où se trouve la donnée à supprimer ( dont l'adresse est le 'i' sortant du module de recherche
        for (m=1;m<=ttaille;m++)//On récupère la taille de la donnée pour la mise à jour de l'entête du nombre de caractères
                                //supprimés
        {
            if (j>=b)//Si la position courante dépasse la taille du bloc
            {
                j=0;
                i=buf.suiv;//On passe au bloc suivant
                liredir(&f,i,&buf);
            }
            taille[m-1]=buf.tab[j];//On récupère le champ taille qui se trouve dans le fichier
            j++;
        }
        taille[ttaille]='\0';//Pour indiquer la fin du champ 'taille'
        tai=atoi(taille);//On convertit la chaine récupérée en entier
        if (j>=b)//Si le champ effacé de la donnée se trouve dans le bloc suivant
        {
            i=buf.suiv;//On passe au bloc suivant
            liredir(&f,i,&buf);
        }
        j=j%b;//On récupère le champ 'effacé' de la donnée
        if (buf.tab[j]=='0')//Donnée non supprimée
        {
            buf.tab[j]='1';//On supprime logiquement la donnée (champ 'effacé' à 1 )
            ecriredir(&f,i,buf);//On met à jour le bloc dans le fichier
            aff_entete(&f,4,entete(&f,4)+tai);//On incrémente le nombre de données supprimées dans l'entête du fichier
        }
        else printf("\nDONNE DEJA SUPPRIMEE !\n");
    }
    else
    {
        printf("\nL'ELEMENT N'EXISTE PAS.\n");
    }
}


/** 5-  **********************************************************************************************/
void reorganisationlnovc(char *nomfich1,char *nomfich2)
//Crée un nouveau fichier contenant les données non supprimées d'un autre fichier
{
    char cle[tcle];
    LNOVC f,g;
    Tbloc buf;
    int i,j,tai,m,k;
    char taille[ttaille];
    char info[b];//La donnée est declarée ainsi de taille fixe, car autrement ça nous a causé des problèmes au cours de
                 //l'éxecution
    ouvrir(&f,nomfich1,"a");//On ouvre le fichier à réorganiser en mode ancien
    ouvrir(&g,nomfich2,"n");//On ouvre le fichier résultat en mode ancien
    i=entete(&f,1);//On récupère l'adresse de la tête du fichier à réorganiser
    j=entete(&f,2);//On récupère l'adresse de la queue du fichier à réorganiser
    k=0;//k représente la variable de parcours du bloc
    if (entete(&f,2)>=1) // Fichier non vide
    {
        liredir(&f,i,&buf);//On récupère la tête
        while (!((i==j)&&(k==entete(&f,3)))&&(i!=-1))//Tant que (on est pas en même temps arrivé à la queue ET à la dernière position de la queue
                                                     //Ou que l'on a pas parcouru tout le fichier
        {
            for (m=1;m<=ttaille;m++)//On récupère la taille de la donnée
            {
                taille[m-1]=buf.tab[k];
                k++;
                if (k>=b)
                {
                    k=0;
                    i=buf.suiv;
                    liredir(&f,i,&buf);
                }
            }
            taille[ttaille]='\0';//Pour indiquer la fin de la taille
            tai=atoi(taille);//On converit la taille en entier
            if (buf.tab[k]=='0') // Donnée non effacée, donc on l'insère dans le nouveau fichier
            {
                for (m=5;m<=tai;m++)
                {
                    k++;
                    if (k>=b)
                    {
                        k=0;
                        i=buf.suiv;
                        liredir(&f,i,&buf);
                    }
                    if ((m>=5)&&(m<=24))//On remplit la clé
                    {
                        cle[m-5]=buf.tab[k];
                    }
                    else
                    {
                        info[m-25]=buf.tab[k];//On remplit la donnée
                        info[m-24]='\0';//La position suivante sera à chaque fois écrasée par le caractère suivant, on sortira
                                        //ainsi avec la dernière position de l'info avec le caractère de fin de chaine
                    }
                }
                taille[ttaille]='\0';//Pour indiquer la fin de la taille
                cle[tcle]='\0';//Pour indiquer la fin de la clé
                info[tai-tcle-4]='\0';//Pour indiquer la fin de la donnée
                //------Insertion dans le nouveau fichier-----------
                reorginsertlnovc(nomfich2,taille,cle,info);//Module fait précedemment d'insertion sans recherche
                //--------------------------------------------------
                k++;//On passe à la positon suivante
            }
            else//Si la donnée a été supprimée
            {
                k=k-ttaille+tai;//On passe à la donnée suivante, car celle-ci ne doit pas être insérée
            }
            if (k>=b)//Si la position dépasse la taille du bloc
            {
                k=(k%b);
                i=buf.suiv;
                liredir(&f,i,&buf);//On passe au bloc suivant
            }
        }
    }
    fermer(&f);
    fermer(&g);
}

/** 6-  **********************************************************************************************/
void charginit (char *nomfich)
//Module qui effectue le chargement initial d'un fichier
{
    char taille[ttaille];
    int n;
    LNOVC f;
    Tbloc buf1,buf,buf2;//buf et buf2 sont déclarées ici malgré leur inutilité dans le programme car les enlever causerait
                        //des erreurs au niveau de l'éxécution
    int i,j,m,tai,p;
    char cle[tcle];
    char donnee[b];//La donnée est declarée ainsi de taille fixe, car autrement ça nous a causé des problèmes au cours de
                   //l'éxecution (effet de bord)
    ouvrir(&f,nomfich,"n");
    printf("ENTRER LE NOMBRE D'ENREGISTREMENTS A INSERER : ");
    scanf("%d",&n);
    j=-1;//j représentera la position dans le buffer, pour le remplir caractère par caractère
    for (i=0;i<n;i++)
    {
        printf("\nL'ENREGISTREMENT NUMERO : %d",i+1);
        printf("\nENTRER LA CLE : ");
        scanf("%s",cle);
        while (strlen(cle)!=tcle)
        {
           printf("\nTAILLE INCOMPATIBLE, VEUILLEZ ENTRER UNE CLE DONT LA TAILLE EST %d : ",tcle);
           scanf("%s",cle);
        }
        printf("\nENTRER LA DONNEE : ");
        scanf("%s",donnee);
        tai=strlen(donnee)+tcle+4;//On récupère la taille de la donnée (+clé et champ effacé et taille)
        itoa(tai,taille,10);//On convertit la taille en entier
        taille[ttaille]='\0';
        if (tai<100)//Pour que les trois positions du champ 'taille' soient toutes pleines
        {
            taille[2]=taille[1];
            taille[1]=taille[0];
            taille[0]='0';
        }
        cle[tcle]='\0';
        for (m=0;m<tai;m++)//On remplit le buffer caractère par caractère
        {
            j++;
            if (j>=b)
            {
                j=0;
                buf.suiv=-1;
                p=allocbloc(&f);
                ecriredir(&f,p,buf);
            }
            if ((m>=0)&&(m<=2))//On remplit le champ 'taille'
            {
                buf.tab[j]=taille[m];
            }
            if (m==3)//On met le champ 'effacé' à 0
            {
                buf.tab[j]='0';
            }
            if ((m>=4)&&(m<=23))//On remplit le champ 'clé'
            {
                buf.tab[j]=cle[m-4];
            }
            if (m>=24)//On remplit le champ 'donnée'
            {
                buf.tab[j]=donnee[m-24];
            };
        }
    }
    buf.suiv=-1;//On chaîne le dernier buffer à NIL
    p=allocbloc(&f);//On alloue un nouveau bloc pour l'insertion du dernier buffer
    ecriredir(&f,p,buf);//On insère le dernier buffer dans le fichier
    aff_entete(&f,3,j+1); // car le j+1 pointe la dérnière position insérée +1 , donc la position libre.
                          //Si le bloc est plein on aura j+1=b+1
    fermer(&f);
}

/** 7-  **********************************************************************************************/
void ecritlnovc (char *nomfich)
//Procédure d'affichage du contenu d'un fichier LNOVC
{
    LNOVC f;
    Tbloc buf;
    int p,i;
    ouvrir(&f,nomfich,"a");
    printf("\n-------------- Affichage du fichier ----------------");
    if ((f.fichier)!=NULL)
    {
        //On commence par afficher les entêtes
        printf("\nEntete 1 (Tete)    : %d",entete(&f,1));
        printf("\nEntete 2 (Queue)   : %d",entete(&f,2));
        printf("\nEntete 3 (Pos_vide): %d",entete(&f,3));
        printf("\nEntete 4 (nb_sup)  : %d",entete(&f,4));
        p=entete(&f,1);//On récupère l'adresse de la tête
        i=0;
        if (p!=-1)
        {
            while (!(((p==entete(&f,2))&&((i==entete(&f,3)))))&&(p!=-1))
                //Tant que (on est pas en même temps arrivé à la queue ET à la dernière position de la queue
                //Ou que l'on a pas parcouru tout le fichier
            {
                liredir(&f,p,&buf);//On récupère le bloc
                printf("\nBloc %d : ",p);
                if (p==entete(&f,2))//Si l'on est arrivé à la queue
                {
                    for (i=0;i<entete(&f,3);i++)//On parcourt la queue jusqu'à la première position vide du fichier
                                                //et non tout le bloc
                    {
                        printf("%c",buf.tab[i]);
                    }
                }
                else
                {
                    for (i=0;i<b;i++)//Nous ne sommes pas à la queue, donc on parcourt tout le bloc
                    {
                    {
                        printf("%c",buf.tab[i]);
                    }
                }
                p=buf.suiv;;//On passe au bloc suivant
            }
        }
        }
        else
        {
            printf("IL N'Y A PAS DE BLOC DE DONNEES");
        }
    }
    printf("\n----------------------------------------------------");
    fermer(&f);
}
/** 8-  **********************************************************************************************/
void reorganisation_avec_index(char *nomfich1,char *nomfich2)
//Crée un nouveau fichier contenant les données non supprimées d'un autre fichier, en créant une table index dense du nouveau fichier
{
    char cle[tcle];
    LNOVC f,g;
    Tbloc buf;
    int i,j,tai,m,k,q,cpt,y,x;
    char taille[ttaille];
    char info[b];//La donnée est declarée ainsi de taille fixe, car autrement ça nous a causé des problèmes au cours de
                 //l'éxecution
    ouvrir(&f,nomfich1,"a");//On ouvre le fichier à réorganiser en mode ancien
    ouvrir(&g,nomfich2,"n");//On ouvre le fichier résultat en mode ancien
    i=entete(&f,1);//On récupère l'adresse de la tête du fichier à réorganiser
    j=entete(&f,2);//On récupère l'adresse de la queue du fichier à réorganiser
    k=0;//k représente la variable de parcours du bloc
    y=0;// y représente la position dans la table d'index
    if (entete(&f,2)>=1) // Fichier non vide
    {
        liredir(&f,i,&buf);//On récupère la tête
        while (!((i==j)&&(k==entete(&f,3)))&&(i!=-1))//Tant que (on est pas en même temps arrivé à la queue ET à la dernière position de la queue
                                                     //Ou que l'on a pas parcouru tout le fichier
        {
            for (m=1;m<=ttaille;m++)//On récupère la taille de la donnée
            {
                taille[m-1]=buf.tab[k];
                x=k;
                q=i;
                k++;
                if (k>=b)
                {
                    k=0;
                    i=buf.suiv;
                    liredir(&f,i,&buf);
                }
            }
            taille[ttaille]='\0';//Pour indiquer la fin de la taille
            tai=atoi(taille);//On converit la taille en entier
            if (buf.tab[k]=='0') // Donnée non effacée, donc on l'insère dans le nouveau fichier
            {
                for (m=5;m<=tai;m++)
                {
                    k++;
                    if (k>=b)
                    {
                        k=0;
                        i=buf.suiv;
                        liredir(&f,i,&buf);
                    }
                    if ((m>=5)&&(m<=24))//On remplit la clé
                    {
                        cle[m-5]=buf.tab[k];
                        index[y].cl[m-5]=buf.tab[k];
                    }
                    else
                    {
                        info[m-25]=buf.tab[k];//On remplit la donnée
                        info[m-24]='\0';//La position suivante sera à chaque fois écrasée par le caractère suivant, on sortira
                                        //ainsi avec la dernière position de l'info avec le caractère de fin de chaine
                    }
                }
                taille[ttaille]='\0';//Pour indiquer la fin de la taille
                cle[tcle]='\0';//Pour indiquer la fin de la clé
                index[y].cl[tcle]='\0';
                info[tai-tcle-4]='\0';//Pour indiquer la fin de la donnée
                //------Insertion dans le nouveau fichier-----------
                reorginsertlnovc_index(nomfich2,taille,cle,info,y);//Module fait précedemment d'insertion sans recherche
                //--------------------------------------------------
                y++;
                k++;//On passe à la positon suivante
            }
            else//Si la donnée a été supprimée
            {
                k=k-ttaille+tai;//On passe à la donnée suivante, car celle-ci ne doit pas être insérée
            }
            if (k>=b)//Si la position dépasse la taille du bloc
            {
                k=(k%b);
                i=buf.suiv;
                liredir(&f,i,&buf);//On passe au bloc suivant
            }
        }
    }
    fermer(&f);
    fermer(&g);
}
void affiche_index()
{
    int y=0;
    printf("LA TABLE INDEX DU FICHIER :\n\n");
    printf("Cle                      Bloc            Pos  \n");
    for (y=0;y<nbI;y++)
    {
        printf("%s     %d               %d\n",index[y].cl,index[y].n_bloc,index[y].posi);
    }
}
void reorginsertlnovc_index(char *nomfich,char taille[ttaille],char cle[tcle],char info[b],int y) // Insertion sans faire la recherche car on est sûr que l'élément n'existe pas
//Tout en mettant à jour l'index du fichier. y représente le numéro de la donnée dans l'index
//Ce module nous sera utile dans la réorganisation du fichier, ainsi ça ne sera pas nécéssaire de rechercher la clé vu qu'on sait
//qu'elle n'existe pas dans le fichier, et en mettant à jour l'index du fichier
{
    LNOVC f;
    Tbloc buf1;
    int m,q,p,k,tai;
    ouvrir(&f,nomfich,"a");
    p=entete(&f,2);//On récupère la queue du fichier
    if (p!=0)
    {
        liredir(&f,p,&buf1);//On récupère le bloc de la queue
    }
    else
    {
        p=allocbloc(&f);//On alloue un nouveau bloc au fichier pour y insérer la donnée
    }
    tai=atoi(taille);//On convertit la chaine récupérée en entier
    k=entete(&f,3);//On récupère la première position vide du fichier, pour y insérer la donnée
    q=p;
    for (m=1;m<=tai;m++)
    {
        if (k>=b)
        {
            k=0;//On passe à la première position du bloc suivant
            q=p;//On sauvegarde l'adresse du bloc précédent
            p=allocbloc(&f);//On alloue un nouveau bloc au fichier pour y insérer la donnée
            if (m==1) index[y].n_bloc=p;//On met à jour le champ n° bloc de l'index avec l'adresse de p, car dans ce cas m=1
                                        //veut dire que notre donnée commence dans le bloc suivant (après allocbloc)
            aff_entete(&f,2,p);//On met à jour la nouvelle queue dans l'entête
            buf1.suiv=p;//On chaîne l'ancienne queue avec la nouvelle
            ecriredir(&f,q,buf1);//On met à jour l'ancienne queue dans le fichier
            aff_entete(&f,3,k);//On met à jour la première position vide du fichier dans l'entête, si jamais on a déjà
                                //inséré toute la donnée
        }
        if ((m>=1)&&(m<=3))
        {
            buf1.tab[k]=taille[m-1];//On remplit le champ 'taille' avec la taille de la donnée
        }
        if (m==4)
        {
            buf1.tab[k]='0';//On met la champ 'effacé' à '0'
        }
        if ((m>=5)&&(m<=24))
        {
            buf1.tab[k]=cle[m-5];//On remplit la champ 'clé' avec la clé à insérer
        }
        if (m>=25)
        {
            buf1.tab[k]=info[m-25];//On remplit le champ 'donnée' avec la donnée à insérer
        }
        if (m==1) index[y].posi=k;//m=1 signifie que la donnée est insérée à la position k du fichier réorganisé, on met donc à jour l'index
        else index[y].n_bloc=q;//Si m!=1,
        k++;//Le remplissage se fait caractère par caractère, donc on passe à la position suivante
            //dans buf
    }
    nbI++;
    buf1.suiv=-1;
    ecriredir(&f,p,buf1);//On écrit le bloc dans le fichier
    aff_entete(&f,3,k); //On met à jour la première position vide, et on met k à sa place
                        //car le k (sortant de la boucle) pointe la dérnière position insérée +1, donc la première position vide.
                        //Si le bloc est plein on aura k+1=b+1
    fermer(&f);
}
void sauvegarde_idx(char *nomfich)
//Sauvegarde la table d'index dans un fichier
{
        Fich_index f;
        int y,k;
        f.fichier=fopen(nomfich,"wb+");
        f.entete_idx.nb_elem=nbI;
        fwrite(&f.entete_idx,sizeof(Tentete_idx),1,f.fichier);//Pour sauvegarder la taille de l'index dans le fichier
        fseek(f.fichier,sizeof(Tentete_idx),SEEK_SET);
        printf("NBI = %d \n",nbI);
        for (y=0;y<nbI;y++)
        {
            printf("%s   %d   %d\n",index[y].cl,index[y].n_bloc,index[y].posi);
            fwrite(&index[y],sizeof(Tindex),1,f.fichier);
            fseek(f.fichier,sizeof(Tindex),SEEK_CUR);
        }

}
void affiche_fich_idx(char *nomfich)
//Affichage du contenu du fichier index
{
    Fich_index f;
    Tindex temp[50];
    int y=0;
    f.fichier=fopen(nomfich,"rb+");
    fseek(f.fichier,sizeof(Tentete_idx),SEEK_SET);
    while (fread(&temp[y],sizeof(Tindex),1,f.fichier)!=0)
    {
        printf("%s   %d   %d\n",temp[y].cl,temp[y].n_bloc,temp[y].posi);
        y++;
    }
}
