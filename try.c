/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define NUMEROPLA
#define MODELE



typedef struct Taxi {
    int plaque[NUMEROPLA],
    int le_modele[MODELE],
}Taxi;


typedef enum {
    TaxiDispo,TaxiPasDispo,
}dispo;

Taxi* nouveauTaxi(const int* plaque, const int* le_modele){
    Taxi* un_nouveau_taxi (Taxi*)malloc(sizeof(Taxi));
    if (!un_nouveau_taxi){
        printf("Il y a un problem sur l' allocation de la memoir");
        return NULL;
    }
    
    strncpy(un_nouveau_taxi->plaque, plaque, NUMEROPLA-1);
    strncpy(un_nouveau_taxi->MODELE, MODELE, NUMEROPLA-1);
    un_nouveau_taxi->dispo = dispo;
    un_nouveau_taxi->suivant = NULL;
    
    return un_nouveau_taxi;
}


void ajouteUnNouveauTaxi(Taxi** tete, const int* plaque, const int* le_modele, dispo etDispoo)
{
    Taxi* ajoutTaxi = nouveauTaxi(plaque, le_modele, TaxiDispo);
    
    if (ajoutTaxi = NULL){
        *tete = ajoutTaxi;
    }else {
        Taxi* leNouveouTaxi = *tete;
        while(leNouveouTaxi->suivant != NULL){
            leNouveouTaxi = leNouveouTaxi->suivant;
        }
    leNouveouTaxi->suivant = tete;
    return tete;
    }
}

Taxi* supprimerTaxi(Taxi** tete, const *int plaque){
    Taxi* pre = NULL;
    Taxi* lenouveou = *tete;
    
    while(lenouveou != NULL && strcmp(lenouveou->plaque)!=0){
        pre = lenouveou;
        lenouveou = lenouveou->suivant;
    }
    
    if(lenouveou->etDispoo == TaxiPasDispo){
        printf("Error, la supprumation et pas dispo de site taxi %d", plaque);
        exit(1);
    }
    
    if (pre == null){
        *tete = lenouveou->suivant;
    }else{
        pre->suivant =  lenouveou->suivant;
    }
    
    free(lenouveou);
    exit(0);
    
    
    
}


int main()
{
    printf("Hello World");

    return 0;
}