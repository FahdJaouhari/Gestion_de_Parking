#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VEHICULES 3
#define NB_NIVEAUX 3
#define MAX_ATTENTE 5

// Structure du véhicule
typedef struct Vehicule{
    char immatriculation[20];
    char modele[20];
    char heure_entree[10];
} Vehicule;

// Pile : Niveau de stationnement
typedef struct Niveau{
    Vehicule voitures[MAX_VEHICULES];
    int sommet;
} Niveau;

// Liste circulaire de niveaux
typedef struct Parking{
    Niveau niveaux[NB_NIVEAUX];
    int courant;
} Parking;

// File pour liste d’attente
typedef struct FileAttente{
    Vehicule file[MAX_ATTENTE];
    int debut,fin,taille;
} FileAttente;

// Fonctions d'initialisation
void initParking(Parking *p) {
    for (int i=0;i<NB_NIVEAUX;i++)
        p->niveaux[i].sommet=-1;
    p->courant=0;
}

void initFile(FileAttente *f) {
    f->debut=0;
    f->fin=0;
    f->taille=0;
}

// Vérifications
int niveauPlein(Niveau *n) {
    return n->sommet==MAX_VEHICULES-1;
}

int niveauVide(Niveau *n) {
    return n->sommet==-1;
}

// Ajout d'un véhicule
void ajouterVehicule(Parking *p, FileAttente *f) {
    Vehicule v;
    int tours=0;
    int ajoute=0;
    printf("Immatriculation: ");
    scanf("%s",v.immatriculation);
    printf("Modele: ");
    scanf("%s",v.modele);
    printf("Heure d'entree: ");
    scanf("%s",v.heure_entree);
    while (tours<NB_NIVEAUX) {
        if (!niveauPlein(&p->niveaux[p->courant])) {
            p->niveaux[p->courant].voitures[++p->niveaux[p->courant].sommet]=v;
            printf("Vehicule gare au niveau %d.\n",p->courant+1);
            ajoute=1;
            break;
        }
        p->courant=(p->courant+1)%NB_NIVEAUX;
        tours++;
    }
    if (!ajoute){
        if (f->taille<MAX_ATTENTE){
            f->file[f->fin]=v;
            f->fin=(f->fin+1)%MAX_ATTENTE;
            f->taille++;
            printf("Parking plein. Vehicule mis en file d'attente.\n");
        } else {
            printf("Parking et file d'attente pleins. Véhicule refuse.\n");
        }
    }
}

// Affichage
void afficherParking(Parking *p){
    for (int i=0;i<NB_NIVEAUX;i++) {
        printf("Niveau %d (%d vehicules) :\n",i+1,p->niveaux[i].sommet+1);
        for (int j=p->niveaux[i].sommet;j>=0;j--){
            Vehicule v=p->niveaux[i].voitures[j];
            printf("  - %s | %s | %s\n",v.immatriculation,v.modele,v.heure_entree);
        }
    }
}

// Retirer dernier véhicule d’un niveau
void retirerVehicule(Parking *p, FileAttente *f){
    int niveau;
    printf("Retirer voiture de quel niveau (1 à %d) ? ", NB_NIVEAUX);
    scanf("%d", &niveau);
    niveau--;
    if (niveau<0 || niveau>=NB_NIVEAUX || niveauVide(&p->niveaux[niveau])){
        printf("Niveau invalide ou vide.\n");
        return;
    }
    Vehicule v=p->niveaux[niveau].voitures[p->niveaux[niveau].sommet--];
    printf("Vehicule retire : %s - %s - %s\n",v.immatriculation,v.modele,v.heure_entree);

    // Si file d'attente non vide
    if (f->taille>0){
        Vehicule en_attente=f->file[f->debut];
        f->debut=(f->debut+1)%MAX_ATTENTE;
        f->taille--;
        p->niveaux[niveau].voitures[++p->niveaux[niveau].sommet]=en_attente;
        printf("Vehicule de la file ajoute : %s\n",en_attente.immatriculation);
    }
}

// Programme principal
int main() {
    Parking parking;
    FileAttente attente;
    initParking(&parking);
    initFile(&attente);

    int choix;
    do {
        printf("\n--- MENU PARKING ---\n");
        printf("1. Ajouter une voiture\n");
        printf("2. Afficher les voitures garees\n");
        printf("3. Retirer une voiture\n");
        printf("0. Quitter\n");
        printf("Choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: ajouterVehicule(&parking, &attente); break;
            case 2: afficherParking(&parking); break;
            case 3: retirerVehicule(&parking, &attente); break;
            case 0: printf("Fin du programme.\n"); break;
            default: printf("Choix invalide.\n");
        }
    } while (choix != 0);

    return 0;
}
