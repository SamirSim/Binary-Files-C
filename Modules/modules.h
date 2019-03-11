#ifndef MODULES_H_INCLUDED
#define MODULES_H_INCLUDED
#include "machine_abstraite.h"

/*------------------------------------------------------------------------------------------------*/
/*                        Entête des modules du TP 2 sur les fichiers LNOVC                       */
/*------------------------------------ -----------------------------------------------------------*/

/** 1-  **************************************************************/
void recherchelnovc(char *nomfich,char cle[tcle],int *trouv,int *i,int *j);

/** 2-  *************************************************************************************/
void insertionlnovc(char *nomfich,char cle[tcle]);

/** 3-  ************************************************************************************/
void reorginsertlnovc(char *nomfich,char taille[ttaille],char cle[tcle],char info[b]); // insertion sans faire la recherche car on est sûr que l'element n'existe pas

/** 4-  **********************************************************************************************/
void suppressionlnovc(char *nomfich,char cle[tcle]);

/** 5-  **********************************************************************************************/
void reorganisationlnovc(char *nomfich1,char *nomfich2);

/** 6-  **********************************************************************************************/
void charginit (char *nomfich);

/** 7-  **********************************************************************************************/
void ecritlnovc (char *nomfich);

#endif // MODULES_H_INCLUDED
