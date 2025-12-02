#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PLAQUE 20
#define MAX_MODELE 50

// Enumération pour l'état du taxi
typedef enum {
    DISPONIBLE,
    EN_COURSE
} EtatTaxi;

// Structure représentant un taxi
typedef struct Taxi {
    char plaque[MAX_PLAQUE];
    char modele[MAX_MODELE];
    EtatTaxi etat;
    struct Taxi* suivant;
} Taxi;

// Fonction pour créer un nouveau taxi
Taxi* creerTaxi(const char* plaque, const char* modele, EtatTaxi etat) {
    Taxi* nouveau = (Taxi*)malloc(sizeof(Taxi));
    if (nouveau == NULL) {
        printf("Erreur d'allocation mémoire!\n");
        return NULL;
    }
    
    strncpy(nouveau->plaque, plaque, MAX_PLAQUE - 1);
    strncpy(nouveau->modele, modele, MAX_MODELE - 1);
    nouveau->etat = etat;
    nouveau->suivant = NULL;
    
    return nouveau;
}

// PARTIE 1 : Liste simplement chaînée pour les taxis disponibles
// =============================================================

// 1. Ajout de taxi à la fin de la liste
void ajouterTaxiDispo(Taxi** tete, const char* plaque, const char* modele) {
    Taxi* nouveau = creerTaxi(plaque, modele, DISPONIBLE);
    if (nouveau == NULL) return;
    
    if (*tete == NULL) {
        *tete = nouveau;
    } else {
        Taxi* courant = *tete;
        while (courant->suivant != NULL) {
            courant = courant->suivant;
        }
        courant->suivant = nouveau;
    }
    printf("Taxi %s (%s) ajouté à la flotte.\n", plaque, modele);
}

// 2. Suppression d'un taxi par sa plaque
int supprimerTaxi(Taxi** tete, const char* plaque) {
    if (*tete == NULL) {
        printf("La flotte est vide!\n");
        return 0;
    }
    
    Taxi* precedent = NULL;
    Taxi* courant = *tete;
    
    // Recherche du taxi à supprimer
    while (courant != NULL && strcmp(courant->plaque, plaque) != 0) {
        precedent = courant;
        courant = courant->suivant;
    }
    
    if (courant == NULL) {
        printf("Taxi avec plaque %s non trouvé.\n", plaque);
        return 0;
    }
    
    // Si le taxi est en course, on ne peut pas le supprimer
    if (courant->etat == EN_COURSE) {
        printf("Impossible de supprimer le taxi %s: il est en course!\n", plaque);
        return 0;
    }
    
    // Suppression du taxi
    if (precedent == NULL) {
        // Suppression de la tête
        *tete = courant->suivant;
    } else {
        precedent->suivant = courant->suivant;
    }
    
    free(courant);
    printf("Taxi %s supprimé de la flotte.\n", plaque);
    return 1;
}

// 3. Modification des informations d'un taxi
int modifierTaxi(Taxi* tete, const char* plaque, const char* nouveauModele) {
    Taxi* courant = tete;
    while (courant != NULL) {
        if (strcmp(courant->plaque, plaque) == 0) {
            strncpy(courant->modele, nouveauModele, MAX_MODELE - 1);
            printf("Modèle du taxi %s modifié en: %s\n", plaque, nouveauModele);
            return 1;
        }
        courant = courant->suivant;
    }
    printf("Taxi avec plaque %s non trouvé.\n", plaque);
    return 0;
}

// 4. Recherche d'un taxi par sa plaque
Taxi* rechercherTaxi(Taxi* tete, const char* plaque) {
    Taxi* courant = tete;
    while (courant != NULL) {
        if (strcmp(courant->plaque, plaque) == 0) {
            return courant;
        }
        courant = courant->suivant;
    }
    return NULL;
}

// 5. Affichage des taxis disponibles
void afficherTaxisDisponibles(Taxi* tete) {
    printf("\n=== TAXIS DISPONIBLES ===\n");
    
    if (tete == NULL) {
        printf("Aucun taxi disponible.\n");
        return;
    }
    
    Taxi* courant = tete;
    int compteur = 0;
    
    while (courant != NULL) {
        if (courant->etat == DISPONIBLE) {
            printf("%d. Plaque: %-10s Modèle: %-15s Etat: Disponible\n",
                   ++compteur, courant->plaque, courant->modele);
        }
        courant = courant->suivant;
    }
    
    if (compteur == 0) {
        printf("Aucun taxi disponible.\n");
    } else {
        printf("Total: %d taxi(s) disponible(s)\n", compteur);
    }
}

// PARTIE 2 : Liste circulaire pour les taxis en service
// ====================================================

// Variable globale pour la tête de la liste circulaire
Taxi* teteCircular = NULL;

// 1. Réservation d'un taxi (ajout à la liste circulaire)
int reserverTaxi(Taxi** teteDispo, const char* plaque) {
    // Recherche du taxi dans la liste des disponibles
    Taxi* taxi = rechercherTaxi(*teteDispo, plaque);
    
    if (taxi == NULL) {
        printf("Taxi %s non trouvé dans la flotte.\n", plaque);
        return 0;
    }
    
    if (taxi->etat == EN_COURSE) {
        printf("Taxi %s est déjà en course!\n", plaque);
        return 0;
    }
    
    // Changement de l'état du taxi
    taxi->etat = EN_COURSE;
    
    // Création d'un nouveau nœud pour la liste circulaire
    Taxi* nouveau = creerTaxi(taxi->plaque, taxi->modele, EN_COURSE);
    if (nouveau == NULL) return 0;
    
    // Ajout à la liste circulaire
    if (teteCircular == NULL) {
        teteCircular = nouveau;
        nouveau->suivant = teteCircular; // Pointe sur lui-même
    } else {
        Taxi* dernier = teteCircular;
        while (dernier->suivant != teteCircular) {
            dernier = dernier->suivant;
        }
        dernier->suivant = nouveau;
        nouveau->suivant = teteCircular;
    }
    
    printf("Taxi %s réservé pour un trajet.\n", plaque);
    return 1;
}

// 2. Fin d'un trajet (rotation dans la liste circulaire)
void terminerTrajet() {
    if (teteCircular == NULL) {
        printf("Aucun taxi en course.\n");
        return;
    }
    
    // Déplacer la tête à la fin (rotation)
    teteCircular = teteCircular->suivant;
    
    // Afficher le taxi qui commence son prochain trajet
    printf("Trajet terminé. Prochain taxi disponible: %s (%s)\n", 
           teteCircular->plaque, teteCircular->modele);
}

// 3. Affichage des taxis en service
void afficherTaxisEnService() {
    printf("\n=== TAXIS EN SERVICE ===\n");
    
    if (teteCircular == NULL) {
        printf("Aucun taxi en service.\n");
        return;
    }
    
    Taxi* courant = teteCircular;
    int compteur = 0;
    
    do {
        printf("%d. Plaque: %-10s Modèle: %-15s Etat: En course\n",
               ++compteur, courant->plaque, courant->modele);
        courant = courant->suivant;
    } while (courant != teteCircular);
    
    printf("Total: %d taxi(s) en service\n", compteur);
}

// Fonction pour libérer toute la mémoire
void libererMemoire(Taxi** teteDispo) {
    // Libération de la liste des disponibles
    Taxi* courant = *teteDispo;
    while (courant != NULL) {
        Taxi* temp = courant;
        courant = courant->suivant;
        free(temp);
    }
    *teteDispo = NULL;
    
    // Libération de la liste circulaire
    if (teteCircular != NULL) {
        Taxi* courant = teteCircular;
        Taxi* temp;
        
        do {
            temp = courant;
            courant = courant->suivant;
            free(temp);
        } while (courant != teteCircular);
        
        teteCircular = NULL;
    }
}

// Fonction pour afficher tous les taxis (disponibles et en service)
void afficherTousTaxis(Taxi* teteDispo) {
    printf("\n=== TOUS LES TAXIS DE LA FLOTTE ===\n");
    
    int compteur = 0;
    Taxi* courant = teteDispo;
    
    while (courant != NULL) {
        printf("%d. Plaque: %-10s Modèle: %-15s Etat: %s\n",
               ++compteur, courant->plaque, courant->modele,
               courant->etat == DISPONIBLE ? "Disponible" : "En course");
        courant = courant->suivant;
    }
    
    printf("Total: %d taxi(s) dans la flotte\n", compteur);
}

// PARTIE 4 : Menu interactif
// ==========================

void afficherMenu() {
    printf("\n=== MENU GESTION TAXIS ===\n");
    printf("1. Ajouter un taxi\n");
    printf("2. Supprimer un taxi\n");
    printf("3. Modifier les informations d'un taxi\n");
    printf("4. Réserver un taxi\n");
    printf("5. Terminer un trajet\n");
    printf("6. Afficher tous les taxis disponibles\n");
    printf("7. Afficher tous les taxis en service\n");
    printf("8. Afficher tous les taxis de la flotte\n");
    printf("9. Quitter\n");
    printf("Choix: ");
}

// Fonction pour valider une plaque (format simplifié)
int validerPlaque(const char* plaque) {
    if (strlen(plaque) == 0 || strlen(plaque) >= MAX_PLAQUE) {
        return 0;
    }
    return 1;
}

// PARTIE 3 : Scénarios de test
// ============================

void testsAutomatiques() {
    printf("\n=== TESTS AUTOMATIQUES ===\n");
    
    Taxi* flotte = NULL;
    
    // Test 1: Ajout de taxis
    printf("\nTest 1: Ajout de taxis...\n");
    ajouterTaxiDispo(&flotte, "AB-123-CD", "Toyota Prius");
    ajouterTaxiDispo(&flotte, "EF-456-GH", "Nissan Leaf");
    ajouterTaxiDispo(&flotte, "IJ-789-KL", "Tesla Model 3");
    
    // Test 2: Affichage des taxis disponibles
    printf("\nTest 2: Affichage des taxis disponibles...\n");
    afficherTaxisDisponibles(flotte);
    
    // Test 3: Réservation d'un taxi
    printf("\nTest 3: Réservation d'un taxi...\n");
    reserverTaxi(&flotte, "AB-123-CD");
    
    // Test 4: Affichage des taxis en service
    printf("\nTest 4: Affichage des taxis en service...\n");
    afficherTaxisEnService();
    
    // Test 5: Modification d'un taxi
    printf("\nTest 5: Modification d'un taxi...\n");
    modifierTaxi(flotte, "EF-456-GH", "Nissan Leaf 2024");
    
    // Test 6: Terminer un trajet
    printf("\nTest 6: Terminaison d'un trajet...\n");
    terminerTrajet();
    
    // Test 7: Suppression d'un taxi
    printf("\nTest 7: Suppression d'un taxi...\n");
    supprimerTaxi(&flotte, "IJ-789-KL");
    
    // Affichage final
    printf("\nTest 8: Situation finale...\n");
    afficherTaxisDisponibles(flotte);
    afficherTaxisEnService();
    
    // Libération de la mémoire
    libererMemoire(&flotte);
}

// Fonction principale
int main() {
    printf("=== SYSTEME DE GESTION DE TAXIS ===\n");
    
    int choix;
    char plaque[MAX_PLAQUE];
    char modele[MAX_MODELE];
    Taxi* flotte = NULL;
    
    // Exécution des tests automatiques
    testsAutomatiques();
    
    printf("\n=== MODE INTERACTIF ===\n");
    
    // Boucle principale du menu
    do {
        afficherMenu();
        scanf("%d", &choix);
        getchar(); // Pour consommer le retour à la ligne
        
        switch (choix) {
            case 1: // Ajouter un taxi
                printf("Numéro de plaque: ");
                fgets(plaque, MAX_PLAQUE, stdin);
                plaque[strcspn(plaque, "\n")] = 0;
                
                printf("Modèle: ");
                fgets(modele, MAX_MODELE, stdin);
                modele[strcspn(modele, "\n")] = 0;
                
                if (validerPlaque(plaque)) {
                    ajouterTaxiDispo(&flotte, plaque, modele);
                } else {
                    printf("Plaque invalide!\n");
                }
                break;
                
            case 2: // Supprimer un taxi
                printf("Numéro de plaque du taxi à supprimer: ");
                fgets(plaque, MAX_PLAQUE, stdin);
                plaque[strcspn(plaque, "\n")] = 0;
                
                supprimerTaxi(&flotte, plaque);
                break;
                
            case 3: // Modifier un taxi
                printf("Numéro de plaque du taxi à modifier: ");
                fgets(plaque, MAX_PLAQUE, stdin);
                plaque[strcspn(plaque, "\n")] = 0;
                
                printf("Nouveau modèle: ");
                fgets(modele, MAX_MODELE, stdin);
                modele[strcspn(modele, "\n")] = 0;
                
                modifierTaxi(flotte, plaque, modele);
                break;
                
            case 4: // Réserver un taxi
                printf("Numéro de plaque du taxi à réserver: ");
                fgets(plaque, MAX_PLAQUE, stdin);
                plaque[strcspn(plaque, "\n")] = 0;
                
                reserverTaxi(&flotte, plaque);
                break;
                
            case 5: // Terminer un trajet
                terminerTrajet();
                break;
                
            case 6: // Afficher taxis disponibles
                afficherTaxisDisponibles(flotte);
                break;
                
            case 7: // Afficher taxis en service
                afficherTaxisEnService();
                break;
                
            case 8: // Afficher tous les taxis
                afficherTousTaxis(flotte);
                break;
                
            case 9: // Quitter
                printf("Au revoir!\n");
                break;
                
            default:
                printf("Choix invalide!\n");
        }
        
    } while (choix != 9);
    
    // Libération de la mémoire avant de quitter
    libererMemoire(&flotte);
    
    return 0;
}