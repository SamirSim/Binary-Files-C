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
//le N� du bloc o� se trouve la cle si elle existe, sinon le N� du bloc ou elle devrait �tre ins�r�e,
//et j la position o� se rouve la cle si elle existe, sinon la position ou elle devrait �tre ins�r�e
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
        p=entete(&f,1);//On r�cup�re l'adresse de la tete du fichier
        k=0;//k va repr�senter la variable de parcours du bloc
        liredir(&f,p,&buf);//On r�cup�re le bloc de la t�te
        while ((p!=-1)&&(*trouv==0))
        {
            temp=k;//On affecte � temp la position courante
            for (m=1;m<=ttaille;m++)
            {
                if (k>=b)//Si la position courante d�passe la taille du bloc
                {
                    k=0;
                    *i=p;//On affecte � i le bloc courant, car le bloc o� se trouve la cl� repr�sente
                         //le bloc o� se trouve le d�but de la donn�e, or la cl� peut se trouver dans
                         //un bloc alors que la donn�e commence dans le bloc pr�c�dent
                    p=buf.suiv;//On passe au bloc suivant
                    liredir(&f,p,&buf);
                }
                taille[m-1]=buf.tab[k];//On r�cup�re le champ taille qui se trouve dans le fichier
                k++;
            }
            taille[ttaille]='\0';//Pour indiquer la fin du champ 'taille'
            tai=atoi(taille);//On convertit la chaine r�cup�r�e en entier
            k++;//On passe � la position suivante, car le champ 'effac�' ne nous int�resse pas ici
            if (k>=b)//A chaque fois qu'on incr�mente la position courante, on doit v�rifier si elle
                    //ne d�passe pas la taille du bloc
            {
                k=0;
                p=buf.suiv;
                liredir(&f,p,&buf);
            }
            for (m=1;m<=tcle;m++)
            {   //On r�cup�re le champ cle qui se trouve dans le fichier
                if (k>=b)
                {
                    k=0;
                    p=buf.suiv;
                    liredir(&f,p,&buf);
                }
                chaine[m-1]=buf.tab[k];
                k++;
            }
            chaine[tcle]='\0';//Pour indiquer la fin de la cl� r�cup�r�e
            cle[tcle]='\0';//Pour indiquer la fin de la cl� recherch�e
            if (strcmp(chaine,cle)==0)//On compare entre la cle r�cup�r�e du fichier et la cl� recherch�e
            {
                *trouv=1;//La cl� est identique, donc cl� trouv�e
            }
            else
            {
                k=k+tai-(tcle+4);//Cl� non trouv�e, donc on passe � l'info suivante en ajoutant � la position
                                 //courante la taille de l'info pr�c�dente-(tcle+24), car la cl� et les
                                 //3 premiers caract�res ne sont pas comptabilis�s
                if (k>=b)
                {   //On passe au d�but de la donn�e suivante, dans le bloc suivant
                    k=k%b;
                    p=buf.suiv;
                    liredir(&f,p,&buf);
                }
                *i=p;//On affecte � i l'adresse du bloc courant
            }
            if ((p==entete(&f,2))&&(k==entete(&f,3))) // On est arriv�s � la premi�re position vide du fichier
            {
                p=-1; // Pour sortir de la boucle
            }
        }
    }
    *j=temp;//On affecte � j temp qui repr�sente la position de la donn�e dans le bloc
     fermer(&f);
}

/** 2-  *************************************************************************************/
void insertionlnovc(char *nomfich,char cle[tcle])
{
    LNOVC f;
    Tbloc buf,buf1,buf2;//buf et buf2 sont d�clar�es ici malgr� leur inutilit� dans le programme car les enlever causerait
                        //des erreurs au niveau de l'�x�cution
    char info[]="";//La donn�e de taille variable, nous n'avons pas utilis� de pointeur car �a cause des probl�mes � la lecture
    int i,j,trouv,taille,m,q,p,k,tai;
    char chaine[ttaille],tail[ttaille];
    recherchelnovc(nomfich,cle,&trouv,&i,&j);//On recherche la cl� dans le fichier
    ouvrir(&f,nomfich,"a");
    if (trouv==1)//Cle existante dans le fichier
    {
        liredir(&f,i,&buf1);//On lit le bloc i du fichier (ou se trouve la cle trouv�e)
        for (m=1;m<=ttaille;m++)//On r�cup�re la taille de la donn�e pour la mise � jour de l'ent�te du nombre de caract�res
                                //supprim�s, si jamais on souhaite ins�rer une donn�e supprim�e
        {
            if (j>=b)//Si la position courante d�passe la taille du bloc
            {
                j=0;
                i=buf1.suiv;//On passe au bloc suivant
                liredir(&f,i,&buf1);
            }
            tail[m-1]=buf1.tab[j];//On r�cup�re le champ taille qui se trouve dans le fichier
            j++;
        }
        tail[ttaille]='\0';//Pour indiquer la fin du champ 'taille'
        tai=atoi(tail);//On convertit la chaine r�cup�r�e en entier
        if (j>=b)//Si le champ effac� de la donn�e se trouve dans le bloc suivant
        {
            i=buf1.suiv;//On passe au bloc suivant
            liredir(&f,i,&buf1);
        }
        j=j%b;//On r�cup�re le champ 'effac�' de la donn�e
        if (buf1.tab[j]=='1')//Si elle a �t� supprim�e
        {
            buf1.tab[j]='0';//On la remet � '0' pour dire qu'elle n'est plus supprim�e
            ecriredir(&f,i,buf1);//On met � jour le bloc dans le fichier
            aff_entete(&f,4,entete(&f,4)-tai);//On met � jour le nombre de caract�res supprim�es dans l'ent�te du fichier
        }
        else
        {
            printf("\nDONNEE EXISTE DEJA.\n");
        }
    }
    else
    {
        p=entete(&f,2);//On r�cup�re la queue du fichier
        if (p!=0)//Si le fichier n'est pas vide
        {
            liredir(&f,p,&buf1);//On r�cup�re le bloc de la queue
        }
        printf("\nENTREZ LA DONNEE A INSERER : ");
        scanf("%s",info);//On r�cup�re l'info � ins�rer
        printf("\nLA DONNEE EST : %s\n",info);
        taille=strlen(info);//On r�cup�re la longueur de l'info
        itoa(taille+tcle+4,chaine,10);//On convertit la taille en chaine de caract�res pour l'insertion
                                      //dans le buffer (taille de l'info + tcle + 3 du champ 'taille'
                                      // + 1 du champ 'effac�'
        chaine[ttaille]='\0';
        if (taille<100)//Pour que les trois positions du champ 'taille' soient toutes pleines
        {
            chaine[2]=chaine[1];
            chaine[1]=chaine[0];
            chaine[0]='0';
        }
        k=entete(&f,3);//On r�cup�re la premi�re position vide du fichier, pour y ins�rer la donn�e
        for (m=1;m<=taille+tcle+4;m++)
        {
            if (k>=b)
            {
                k=0;//On passe � la premi�re position du bloc suivant
                q=p;//On sauvegarde l'adresse du bloc pr�c�dent
                p=allocbloc(&f);//On alloue un nouveau bloc au fichier pour y ins�rer la donn�e
                aff_entete(&f,2,p);//On met � jour la nouvelle queue dans l'ent�te
                buf1.suiv=p;//On cha�ne l'ancienne queue avec la nouvelle
                ecriredir(&f,q,buf1);//On met � jour l'ancienne queue dans le fichier
                aff_entete(&f,3,k);//On met � jour l'adresse de la queue dans l'ent�te
            }
            if ((m>=1)&&(m<=3))
            {
                buf1.tab[k]=chaine[m-1];//On remplit le champ 'taille' avec la taille de la donn�e
            }
            if (m==4)
            {
                buf1.tab[k]='0';//On met la champ 'effac�' � '0'
            }
            if ((m>=5)&&(m<=24))
            {
                buf1.tab[k]=cle[m-5];//On remplit la champ 'cl�' avec la cl� � ins�rer
            }
            if (m>=25)
            {
                buf1.tab[k]=info[m-25];//On remplit le champ 'donn�e' avec la donn�e � ins�rer
            }
            k++;//Le remplissage se fait caract�re par caract�re, donc on passe � la position suivante
                //dans buf
        }
        buf1.suiv=-1;//On cha�ne la nouvelle queue � NIL
        ecriredir(&f,p,buf1);//On �crit la derni�re queue dans le fichier
        aff_entete(&f,3,k); //On met � jour la premi�re position vide, et on met k � sa place
                            //car le k pointe la d�rni�re position ins�r�e +1 . Si le bloc est plein on aura j+1=b+1
    }
    fermer(&f);
}

/** 3-  ************************************************************************************/
void reorginsertlnovc(char *nomfich,char taille[ttaille],char cle[tcle],char info[b]) // Insertion sans faire la recherche car on est s�r que l'�l�ment n'existe pas
//Ce module nous sera utile dans la r�organisation du fichier, ainsi �a ne sera pas n�c�ssaire de rechercher la cl� vu qu'on sait
//qu'elle n'existe pas dans le fichier
{
    LNOVC f;
    Tbloc buf1;
    int m,q,p,k,tai;
    ouvrir(&f,nomfich,"a");
    p=entete(&f,2);//On r�cup�re la queue du fichier
    if (p!=0)
    {
        liredir(&f,p,&buf1);//On r�cup�re le bloc de la queue
    }
    else
    {
        p=allocbloc(&f);//On alloue un nouveau bloc au fichier pour y ins�rer la donn�e
    }
    tai=atoi(taille);//On convertit la chaine r�cup�r�e en entier
    k=entete(&f,3);//On r�cup�re la premi�re position vide du fichier, pour y ins�rer la donn�e
    for (m=1;m<=tai;m++)
    {
        if (k>=b)
        {
            k=0;//On passe � la premi�re position du bloc suivant
            q=p;//On sauvegarde l'adresse du bloc pr�c�dent
            p=allocbloc(&f);//On alloue un nouveau bloc au fichier pour y ins�rer la donn�e
            aff_entete(&f,2,p);//On met � jour la nouvelle queue dans l'ent�te
            buf1.suiv=p;//On cha�ne l'ancienne queue avec la nouvelle
            ecriredir(&f,q,buf1);//On met � jour l'ancienne queue dans le fichier
            aff_entete(&f,3,k);//On met � jour la premi�re position vide du fichier dans l'ent�te, si jamais on a d�j�
                                //ins�r� toute la donn�e
        }
        if ((m>=1)&&(m<=3))
        {
            buf1.tab[k]=taille[m-1];//On remplit le champ 'taille' avec la taille de la donn�e
        }
        if (m==4)
        {
            buf1.tab[k]='0';//On met la champ 'effac�' � '0'
        }
        if ((m>=5)&&(m<=24))
        {
            buf1.tab[k]=cle[m-5];//On remplit la champ 'cl�' avec la cl� � ins�rer
        }
        if (m>=25)
        {
            buf1.tab[k]=info[m-25];//On remplit le champ 'donn�e' avec la donn�e � ins�rer
        }
        k++;//Le remplissage se fait caract�re par caract�re, donc on passe � la position suivante
            //dans buf
    }
    buf1.suiv=-1;
    ecriredir(&f,p,buf1);//On �crit le bloc dans le fichier
    aff_entete(&f,3,k); //On met � jour la premi�re position vide, et on met k � sa place
                        //car le k (sortant de la boucle) pointe la d�rni�re position ins�r�e +1, donc la premi�re position vide.
                        //Si le bloc est plein on aura k+1=b+1
    fermer(&f);
}

/** 4-  **********************************************************************************************/
void suppressionlnovc(char *nomfich,char cle[tcle])
//Effectue la suppression logique de la donn�e dont la cl� est 'cle' dans le fichier
{
    LNOVC f;
    Tbloc buf;
    int tai,m,i,j,trouv;
    char taille[ttaille];
    recherchelnovc(nomfich,cle,&trouv,&i,&j);//On recherche la donn�e dans le fichier
    if (trouv==1)// Si donn�e trouv�e
    {
        ouvrir(&f,nomfich,"a");
        liredir(&f,i,&buf);//On r�cup�re le bloc o� se trouve la donn�e � supprimer ( dont l'adresse est le 'i' sortant du module de recherche
        for (m=1;m<=ttaille;m++)//On r�cup�re la taille de la donn�e pour la mise � jour de l'ent�te du nombre de caract�res
                                //supprim�s
        {
            if (j>=b)//Si la position courante d�passe la taille du bloc
            {
                j=0;
                i=buf.suiv;//On passe au bloc suivant
                liredir(&f,i,&buf);
            }
            taille[m-1]=buf.tab[j];//On r�cup�re le champ taille qui se trouve dans le fichier
            j++;
        }
        taille[ttaille]='\0';//Pour indiquer la fin du champ 'taille'
        tai=atoi(taille);//On convertit la chaine r�cup�r�e en entier
        if (j>=b)//Si le champ effac� de la donn�e se trouve dans le bloc suivant
        {
            i=buf.suiv;//On passe au bloc suivant
            liredir(&f,i,&buf);
        }
        j=j%b;//On r�cup�re le champ 'effac�' de la donn�e
        if (buf.tab[j]=='0')//Donn�e non supprim�e
        {
            buf.tab[j]='1';//On supprime logiquement la donn�e (champ 'effac�' � 1 )
            ecriredir(&f,i,buf);//On met � jour le bloc dans le fichier
            aff_entete(&f,4,entete(&f,4)+tai);//On incr�mente le nombre de donn�es supprim�es dans l'ent�te du fichier
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
//Cr�e un nouveau fichier contenant les donn�es non supprim�es d'un autre fichier
{
    char cle[tcle];
    LNOVC f,g;
    Tbloc buf;
    int i,j,tai,m,k;
    char taille[ttaille];
    char info[b];//La donn�e est declar�e ainsi de taille fixe, car autrement �a nous a caus� des probl�mes au cours de
                 //l'�xecution
    ouvrir(&f,nomfich1,"a");//On ouvre le fichier � r�organiser en mode ancien
    ouvrir(&g,nomfich2,"n");//On ouvre le fichier r�sultat en mode ancien
    i=entete(&f,1);//On r�cup�re l'adresse de la t�te du fichier � r�organiser
    j=entete(&f,2);//On r�cup�re l'adresse de la queue du fichier � r�organiser
    k=0;//k repr�sente la variable de parcours du bloc
    if (entete(&f,2)>=1) // Fichier non vide
    {
        liredir(&f,i,&buf);//On r�cup�re la t�te
        while (!((i==j)&&(k==entete(&f,3)))&&(i!=-1))//Tant que (on est pas en m�me temps arriv� � la queue ET � la derni�re position de la queue
                                                     //Ou que l'on a pas parcouru tout le fichier
        {
            for (m=1;m<=ttaille;m++)//On r�cup�re la taille de la donn�e
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
            if (buf.tab[k]=='0') // Donn�e non effac�e, donc on l'ins�re dans le nouveau fichier
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
                    if ((m>=5)&&(m<=24))//On remplit la cl�
                    {
                        cle[m-5]=buf.tab[k];
                    }
                    else
                    {
                        info[m-25]=buf.tab[k];//On remplit la donn�e
                        info[m-24]='\0';//La position suivante sera � chaque fois �cras�e par le caract�re suivant, on sortira
                                        //ainsi avec la derni�re position de l'info avec le caract�re de fin de chaine
                    }
                }
                taille[ttaille]='\0';//Pour indiquer la fin de la taille
                cle[tcle]='\0';//Pour indiquer la fin de la cl�
                info[tai-tcle-4]='\0';//Pour indiquer la fin de la donn�e
                //------Insertion dans le nouveau fichier-----------
                reorginsertlnovc(nomfich2,taille,cle,info);//Module fait pr�cedemment d'insertion sans recherche
                //--------------------------------------------------
                k++;//On passe � la positon suivante
            }
            else//Si la donn�e a �t� supprim�e
            {
                k=k-ttaille+tai;//On passe � la donn�e suivante, car celle-ci ne doit pas �tre ins�r�e
            }
            if (k>=b)//Si la position d�passe la taille du bloc
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
    Tbloc buf1,buf,buf2;//buf et buf2 sont d�clar�es ici malgr� leur inutilit� dans le programme car les enlever causerait
                        //des erreurs au niveau de l'�x�cution
    int i,j,m,tai,p;
    char cle[tcle];
    char donnee[b];//La donn�e est declar�e ainsi de taille fixe, car autrement �a nous a caus� des probl�mes au cours de
                   //l'�xecution (effet de bord)
    ouvrir(&f,nomfich,"n");
    printf("ENTRER LE NOMBRE D'ENREGISTREMENTS A INSERER : ");
    scanf("%d",&n);
    j=-1;//j repr�sentera la position dans le buffer, pour le remplir caract�re par caract�re
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
        tai=strlen(donnee)+tcle+4;//On r�cup�re la taille de la donn�e (+cl� et champ effac� et taille)
        itoa(tai,taille,10);//On convertit la taille en entier
        taille[ttaille]='\0';
        if (tai<100)//Pour que les trois positions du champ 'taille' soient toutes pleines
        {
            taille[2]=taille[1];
            taille[1]=taille[0];
            taille[0]='0';
        }
        cle[tcle]='\0';
        for (m=0;m<tai;m++)//On remplit le buffer caract�re par caract�re
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
            if (m==3)//On met le champ 'effac�' � 0
            {
                buf.tab[j]='0';
            }
            if ((m>=4)&&(m<=23))//On remplit le champ 'cl�'
            {
                buf.tab[j]=cle[m-4];
            }
            if (m>=24)//On remplit le champ 'donn�e'
            {
                buf.tab[j]=donnee[m-24];
            };
        }
    }
    buf.suiv=-1;//On cha�ne le dernier buffer � NIL
    p=allocbloc(&f);//On alloue un nouveau bloc pour l'insertion du dernier buffer
    ecriredir(&f,p,buf);//On ins�re le dernier buffer dans le fichier
    aff_entete(&f,3,j+1); // car le j+1 pointe la d�rni�re position ins�r�e +1 , donc la position libre.
                          //Si le bloc est plein on aura j+1=b+1
    fermer(&f);
}

/** 7-  **********************************************************************************************/
void ecritlnovc (char *nomfich)
//Proc�dure d'affichage du contenu d'un fichier LNOVC
{
    LNOVC f;
    Tbloc buf;
    int p,i;
    ouvrir(&f,nomfich,"a");
    printf("\n-------------- Affichage du fichier ----------------");
    if ((f.fichier)!=NULL)
    {
        //On commence par afficher les ent�tes
        printf("\nEntete 1 (Tete)    : %d",entete(&f,1));
        printf("\nEntete 2 (Queue)   : %d",entete(&f,2));
        printf("\nEntete 3 (Pos_vide): %d",entete(&f,3));
        printf("\nEntete 4 (nb_sup)  : %d",entete(&f,4));
        p=entete(&f,1);//On r�cup�re l'adresse de la t�te
        i=0;
        if (p!=-1)
        {
            while (!(((p==entete(&f,2))&&((i==entete(&f,3)))))&&(p!=-1))
                //Tant que (on est pas en m�me temps arriv� � la queue ET � la derni�re position de la queue
                //Ou que l'on a pas parcouru tout le fichier
            {
                liredir(&f,p,&buf);//On r�cup�re le bloc
                printf("\nBloc %d : ",p);
                if (p==entete(&f,2))//Si l'on est arriv� � la queue
                {
                    for (i=0;i<entete(&f,3);i++)//On parcourt la queue jusqu'� la premi�re position vide du fichier
                                                //et non tout le bloc
                    {
                        printf("%c",buf.tab[i]);
                    }
                }
                else
                {
                    for (i=0;i<b;i++)//Nous ne sommes pas � la queue, donc on parcourt tout le bloc
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
//Cr�e un nouveau fichier contenant les donn�es non supprim�es d'un autre fichier, en cr�ant une table index dense du nouveau fichier
{
    char cle[tcle];
    LNOVC f,g;
    Tbloc buf;
    int i,j,tai,m,k,q,cpt,y,x;
    char taille[ttaille];
    char info[b];//La donn�e est declar�e ainsi de taille fixe, car autrement �a nous a caus� des probl�mes au cours de
                 //l'�xecution
    ouvrir(&f,nomfich1,"a");//On ouvre le fichier � r�organiser en mode ancien
    ouvrir(&g,nomfich2,"n");//On ouvre le fichier r�sultat en mode ancien
    i=entete(&f,1);//On r�cup�re l'adresse de la t�te du fichier � r�organiser
    j=entete(&f,2);//On r�cup�re l'adresse de la queue du fichier � r�organiser
    k=0;//k repr�sente la variable de parcours du bloc
    y=0;// y repr�sente la position dans la table d'index
    if (entete(&f,2)>=1) // Fichier non vide
    {
        liredir(&f,i,&buf);//On r�cup�re la t�te
        while (!((i==j)&&(k==entete(&f,3)))&&(i!=-1))//Tant que (on est pas en m�me temps arriv� � la queue ET � la derni�re position de la queue
                                                     //Ou que l'on a pas parcouru tout le fichier
        {
            for (m=1;m<=ttaille;m++)//On r�cup�re la taille de la donn�e
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
            if (buf.tab[k]=='0') // Donn�e non effac�e, donc on l'ins�re dans le nouveau fichier
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
                    if ((m>=5)&&(m<=24))//On remplit la cl�
                    {
                        cle[m-5]=buf.tab[k];
                        index[y].cl[m-5]=buf.tab[k];
                    }
                    else
                    {
                        info[m-25]=buf.tab[k];//On remplit la donn�e
                        info[m-24]='\0';//La position suivante sera � chaque fois �cras�e par le caract�re suivant, on sortira
                                        //ainsi avec la derni�re position de l'info avec le caract�re de fin de chaine
                    }
                }
                taille[ttaille]='\0';//Pour indiquer la fin de la taille
                cle[tcle]='\0';//Pour indiquer la fin de la cl�
                index[y].cl[tcle]='\0';
                info[tai-tcle-4]='\0';//Pour indiquer la fin de la donn�e
                //------Insertion dans le nouveau fichier-----------
                reorginsertlnovc_index(nomfich2,taille,cle,info,y);//Module fait pr�cedemment d'insertion sans recherche
                //--------------------------------------------------
                y++;
                k++;//On passe � la positon suivante
            }
            else//Si la donn�e a �t� supprim�e
            {
                k=k-ttaille+tai;//On passe � la donn�e suivante, car celle-ci ne doit pas �tre ins�r�e
            }
            if (k>=b)//Si la position d�passe la taille du bloc
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
void reorginsertlnovc_index(char *nomfich,char taille[ttaille],char cle[tcle],char info[b],int y) // Insertion sans faire la recherche car on est s�r que l'�l�ment n'existe pas
//Tout en mettant � jour l'index du fichier. y repr�sente le num�ro de la donn�e dans l'index
//Ce module nous sera utile dans la r�organisation du fichier, ainsi �a ne sera pas n�c�ssaire de rechercher la cl� vu qu'on sait
//qu'elle n'existe pas dans le fichier, et en mettant � jour l'index du fichier
{
    LNOVC f;
    Tbloc buf1;
    int m,q,p,k,tai;
    ouvrir(&f,nomfich,"a");
    p=entete(&f,2);//On r�cup�re la queue du fichier
    if (p!=0)
    {
        liredir(&f,p,&buf1);//On r�cup�re le bloc de la queue
    }
    else
    {
        p=allocbloc(&f);//On alloue un nouveau bloc au fichier pour y ins�rer la donn�e
    }
    tai=atoi(taille);//On convertit la chaine r�cup�r�e en entier
    k=entete(&f,3);//On r�cup�re la premi�re position vide du fichier, pour y ins�rer la donn�e
    q=p;
    for (m=1;m<=tai;m++)
    {
        if (k>=b)
        {
            k=0;//On passe � la premi�re position du bloc suivant
            q=p;//On sauvegarde l'adresse du bloc pr�c�dent
            p=allocbloc(&f);//On alloue un nouveau bloc au fichier pour y ins�rer la donn�e
            if (m==1) index[y].n_bloc=p;//On met � jour le champ n� bloc de l'index avec l'adresse de p, car dans ce cas m=1
                                        //veut dire que notre donn�e commence dans le bloc suivant (apr�s allocbloc)
            aff_entete(&f,2,p);//On met � jour la nouvelle queue dans l'ent�te
            buf1.suiv=p;//On cha�ne l'ancienne queue avec la nouvelle
            ecriredir(&f,q,buf1);//On met � jour l'ancienne queue dans le fichier
            aff_entete(&f,3,k);//On met � jour la premi�re position vide du fichier dans l'ent�te, si jamais on a d�j�
                                //ins�r� toute la donn�e
        }
        if ((m>=1)&&(m<=3))
        {
            buf1.tab[k]=taille[m-1];//On remplit le champ 'taille' avec la taille de la donn�e
        }
        if (m==4)
        {
            buf1.tab[k]='0';//On met la champ 'effac�' � '0'
        }
        if ((m>=5)&&(m<=24))
        {
            buf1.tab[k]=cle[m-5];//On remplit la champ 'cl�' avec la cl� � ins�rer
        }
        if (m>=25)
        {
            buf1.tab[k]=info[m-25];//On remplit le champ 'donn�e' avec la donn�e � ins�rer
        }
        if (m==1) index[y].posi=k;//m=1 signifie que la donn�e est ins�r�e � la position k du fichier r�organis�, on met donc � jour l'index
        else index[y].n_bloc=q;//Si m!=1,
        k++;//Le remplissage se fait caract�re par caract�re, donc on passe � la position suivante
            //dans buf
    }
    nbI++;
    buf1.suiv=-1;
    ecriredir(&f,p,buf1);//On �crit le bloc dans le fichier
    aff_entete(&f,3,k); //On met � jour la premi�re position vide, et on met k � sa place
                        //car le k (sortant de la boucle) pointe la d�rni�re position ins�r�e +1, donc la premi�re position vide.
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
